"""Segment blood vessels (dark anechoic structures) from ultrasound frames.

Pure image processing with OpenCV only. Three complementary detectors:

A1) Small interior vessels
    Plain connected components of the dark mask that do NOT touch any image
    border (catches small vessels that erosion would destroy).

A2) Interior + edge-touching vessels joined to the margin by a thin neck
    1. Gaussian blur            -> suppresses speckle noise
    2. Low-intensity threshold  -> vessel interiors are anechoic (near-black);
                                    a fixed low threshold is more robust than
                                    Otsu on dark frames, where Otsu's split
                                    point lands inside the tissue
    3. Morphological cleanup    -> removes isolated noise pixels
    4. Erosion                  -> thin out-of-scan shadow strips vanish; thick
                                    vessel cores survive as "seeds"
    5. Border filter            -> seeds still touching an image edge belong to
                                    out-of-scan margins / probe artifacts and
                                    are dropped (edge-cut vessels are handled
                                    by detector B)
    6. Dilation + candidate test-> seeds dilated back to full vessel extent,
                                    kept only if they pass the vessel filters

B) Edge-cut (partial) vessels
   A vessel cut off by the scan border fuses with the dark margin strip and
   cannot be split by erosion. It is detected via a width-profile analysis:
   for each image border, measure how far the dark region penetrates inward
   per row/column; a real vessel bulges out beyond the (roughly constant)
   strip width over a compact range of rows.

Usage:
    python 2_segment/segment_vessels.py                          # defaults
    python 2_segment/segment_vessels.py <input_dir>              # custom input dir
    python 2_segment/segment_vessels.py <input_dir> <output_dir> # both
    add --overwrite to reprocess frames that already have outputs

Default input : data/video2images/<video_stem>/   (all *_frame_*.png)
Default output: 2_segment/output/<video_stem>/masks/     binary masks
                2_segment/output/<video_stem>/overlays/  red-highlighted overlays
                2_segment/output/<video_stem>/json/      per-frame component JSON
"""

import argparse
import json
from pathlib import Path

import cv2
import numpy as np

PROJECT_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_INPUT_DIR = PROJECT_ROOT / "data" / "video2images"
DEFAULT_OUTPUT_DIR = PROJECT_ROOT / "2_segment" / "output"

# --- Tunable parameters -------------------------------------------------
DARK_THRESHOLD = 15     # pixel intensity below this counts as "vessel" (0-255)
MIN_SEED_AREA = 200     # min area of an eroded seed to be considered

MIN_AREA_FRAC = 0.0025  # ignore vessels smaller than 0.25% of the image
MAX_AREA_FRAC = 0.10    # ignore regions larger than 10% (out-of-scan margins)
MIN_SOLIDITY = 0.35     # ignore ragged speckle clusters; vessels are compact
MAX_ELONGATION = 4.0    # ignore thin strips/wedges
MAX_MEDIAN_INTENSITY = 8  # vessel interiors are anechoic (median <= ~5);
                          # speckle clusters have a brighter median

# Border-bulge detector (partial vessels cut off by the scan border)
BULGE_BASELINE_PCT = 75   # strip width baseline = this percentile of penetrations
BULGE_DELTA = 15          # extra penetration (px) that counts as a bulge
BULGE_MIN_ROWS = 20       # min consecutive rows/cols for a bulge to count
BULGE_MAX_FRAC = 0.25     # max fraction of the border a bulge may span
MIN_MARGIN_WIDTH = 18     # only analyse borders whose strip is at least this wide
MAX_MARGIN_WIDTH = 90     # ... and at most this wide (wider => tapering wedge)


def erode_radius(h: int, w: int) -> int:
    """Erosion radius scaled to image size: large enough to remove thin shadow
    strips (~10-40 px), small enough to keep vessel cores intact."""
    return max(4, min(h, w) // 64)


def _penetration(binary: np.ndarray, border: str) -> np.ndarray:
    """Per-line run length of dark pixels starting at the given image border.

    Returns an array indexed by line (rows for left/right borders, columns
    for top/bottom borders).
    """
    h, w = binary.shape
    if border == "left":
        src = ~binary
    elif border == "right":
        src = ~binary[:, ::-1]
    elif border == "top":
        src = (~binary).T
    else:  # bottom
        src = (~binary[::-1, :]).T

    first_false = src.argmax(axis=1)
    return np.where(src.any(axis=1), first_false, w if border in ("left", "right") else h)


def _consecutive_runs(flags: np.ndarray):
    """Yield (start, end) index pairs of consecutive True runs."""
    start = None
    for i, f in enumerate(flags):
        if f and start is None:
            start = i
        elif not f and start is not None:
            yield start, i
            start = None
    if start is not None:
        yield start, len(flags)


def _is_vessel_candidate(gray: np.ndarray, mask: np.ndarray, img_area: float) -> bool:
    """Area / intensity / solidity / elongation filters for a vessel candidate."""
    area = int(cv2.countNonZero(mask))
    if not (MIN_AREA_FRAC * img_area <= area <= MAX_AREA_FRAC * img_area):
        return False
    # Vessel interiors are anechoic; speckle clusters are brighter. The median
    # is robust to the few bright pixels of the echogenic vessel wall.
    if float(np.median(gray[mask > 0])) > MAX_MEDIAN_INTENSITY:
        return False
    x, y, cw, ch = cv2.boundingRect(mask)
    elong = max(cw / ch, ch / cw) if min(cw, ch) > 0 else 99
    if elong > MAX_ELONGATION:
        return False
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    hull_area = cv2.contourArea(cv2.convexHull(contours[0])) if contours else 0.0
    solidity = area / (hull_area + 1e-6) if contours else 0.0
    return solidity >= MIN_SOLIDITY


def _interior_vessels(gray: np.ndarray, cleaned: np.ndarray, h: int, w: int) -> np.ndarray:
    """Detectors A1 + A2 unioned."""
    img_area = float(h * w)
    mask = np.zeros((h, w), dtype=np.uint8)

    # A1. Plain connected components that do not touch the image border.
    n_comp, comp_labels, comp_stats, _ = cv2.connectedComponentsWithStats(
        cleaned, connectivity=8
    )
    for i in range(1, n_comp):  # label 0 is background
        x, y, cw, ch, area = comp_stats[i]
        if (x == 0) or (y == 0) or (x + cw >= w) or (y + ch >= h):
            continue
        comp_mask = (comp_labels == i).astype(np.uint8) * 255
        if _is_vessel_candidate(gray, comp_mask, img_area):
            mask |= comp_mask

    # A2. Erode to seeds, drop border-spanning margins, dilate back.
    r = erode_radius(h, w)
    big_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2 * r + 1, 2 * r + 1))
    seeds = cv2.erode(cleaned, big_kernel)

    n_seeds, seed_labels, seed_stats, _ = cv2.connectedComponentsWithStats(
        seeds, connectivity=8
    )
    for i in range(1, n_seeds):  # label 0 is background
        x, y, cw, ch, area = seed_stats[i]
        if area < MIN_SEED_AREA:
            continue
        # Seeds still touching the image border belong to out-of-scan margins
        # or near-field probe artifacts. Vessels attached to the margin only
        # through a thin strip got disconnected by erosion; vessels cut off
        # by the scan border are handled by detector B (border bulge).
        if x == 0 or y == 0 or x + cw >= w or y + ch >= h:
            continue
        seed_region = cv2.dilate((seed_labels == i).astype(np.uint8) * 255, big_kernel)
        if _is_vessel_candidate(gray, seed_region, img_area):
            mask |= seed_region
    return mask


def _border_bulge_vessels(gray: np.ndarray, cleaned: np.ndarray, h: int, w: int) -> list:
    """Detector B: partial vessels cut off by the scan border.

    For each image border that has a real margin strip (width within
    [MIN_MARGIN_WIDTH, MAX_MARGIN_WIDTH]), find rows/columns where the dark
    region penetrates significantly beyond the baseline strip width; those
    "excess" pixels are candidate vessels.
    """
    img_area = float(h * w)
    candidates = []

    for border in ("left", "right", "top", "bottom"):
        runs = _penetration(cleaned > 0, border)
        active = runs[runs > 0]
        if len(active) < BULGE_MIN_ROWS:
            continue
        baseline = float(np.percentile(active, BULGE_BASELINE_PCT))
        if not (MIN_MARGIN_WIDTH <= baseline <= MAX_MARGIN_WIDTH):
            continue

        border_len = h if border in ("left", "right") else w
        flags = runs > baseline + BULGE_DELTA
        for start, end in _consecutive_runs(flags):
            length = end - start
            if not (BULGE_MIN_ROWS <= length <= int(BULGE_MAX_FRAC * border_len)):
                continue

            cand = np.zeros((h, w), dtype=np.uint8)
            base = int(baseline)
            for p in range(start, end):
                depth = int(runs[p])
                if depth <= base:
                    continue
                if border == "left":
                    cand[p, base:depth] = 255
                elif border == "right":
                    cand[p, w - depth:w - base] = 255
                elif border == "top":
                    cand[base:depth, p] = 255
                else:  # bottom
                    cand[h - depth:h - base, p] = 255

            if _is_vessel_candidate(gray, cand, img_area):
                candidates.append(cand)
    return candidates


def segment_vessel(gray: np.ndarray):
    """Return (mask, stats_dict) for one grayscale ultrasound frame."""
    h, w = gray.shape[:2]
    img_area = float(h * w)

    # 1. Blur to suppress speckle so isolated noise pixels don't form clusters.
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)

    # 2. Threshold: vessel interiors are anechoic (near-black). A fixed low
    # threshold is more robust than Otsu on dark ultrasound frames.
    thresh = np.where(blurred < DARK_THRESHOLD, 255, 0).astype(np.uint8)

    # 3. Morphological cleanup: kill isolated noise pixels, close small gaps.
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
    cleaned = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
    cleaned = cv2.morphologyEx(cleaned, cv2.MORPH_CLOSE, kernel)

    # A) Interior vessels (small + edge-touching).
    mask = _interior_vessels(gray, cleaned, h, w)

    # B) Edge-cut partial vessels (skip if they overlap an existing vessel).
    for cand in _border_bulge_vessels(gray, cleaned, h, w):
        cand_area = int(cv2.countNonZero(cand))
        overlap = int(cv2.countNonZero(cv2.bitwise_and(mask, cand)))
        if overlap / max(cand_area, 1) > 0.3:
            continue
        mask |= cand

    # Final component stats on the merged mask.
    n_labels, labels, stats, _ = cv2.connectedComponentsWithStats(mask, connectivity=8)
    components = []
    for i in range(1, n_labels):  # label 0 is background
        x, y, cw, ch, area = stats[i]
        if not (MIN_AREA_FRAC * img_area <= area <= MAX_AREA_FRAC * img_area):
            continue
        comp_mask = (labels == i).astype(np.uint8) * 255
        contours, _ = cv2.findContours(comp_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        hull_area = cv2.contourArea(cv2.convexHull(contours[0])) if contours else 0.0
        solidity = area / (hull_area + 1e-6) if contours else 0.0
        aspect = cw / ch if ch > 0 else 0.0

        components.append(
            {
                "id": len(components) + 1,
                "bbox": [int(x), int(y), int(cw), int(ch)],
                "area_px": int(area),
                "area_frac": round(area / img_area, 4),
                "solidity": round(float(solidity), 3),
                "aspect_ratio_w_h": round(float(aspect), 2),
            }
        )

    stats_out = {
        "image_size": [int(w), int(h)],
        "num_vessels": len(components),
        "total_vessel_area_px": int(cv2.countNonZero(mask)),
        "components": components,
    }
    return mask, stats_out


def make_overlay(gray: np.ndarray, mask: np.ndarray) -> np.ndarray:
    """Red-highlight the vessel mask over a BGR version of the frame."""
    bgr = cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR)
    overlay = bgr.copy()
    overlay[mask > 0] = (0, 0, 255)  # red in BGR
    return cv2.addWeighted(bgr, 0.7, overlay, 0.3, 0)


def main() -> None:
    parser = argparse.ArgumentParser(description="Segment vessels from ultrasound frames.")
    parser.add_argument("input_dir", nargs="?", default=str(DEFAULT_INPUT_DIR))
    parser.add_argument("output_dir", nargs="?", default=str(DEFAULT_OUTPUT_DIR))
    parser.add_argument("--overwrite", action="store_true", help="reprocess existing frames")
    args = parser.parse_args()

    input_root = Path(args.input_dir)
    output_root = Path(args.output_dir)

    # Discover frame directories (each contains *_frame_*.png files).
    # Accept either a direct frame dir or a parent dir containing frame dirs.
    if list(input_root.glob("*_frame_*.png")):
        frame_dirs = [input_root]
    else:
        frame_dirs = sorted(
            d for d in input_root.iterdir()
            if d.is_dir() and list(d.glob("*_frame_*.png"))
        )

    if not frame_dirs:
        raise SystemExit(f"No frame directories with *_frame_*.png found under {input_root}")

    summary = {"processed_frames": 0, "frames_with_vessels": 0, "per_frame": []}

    for frame_dir in frame_dirs:
        stem = frame_dir.name
        out_dir = output_root / stem
        masks_dir = out_dir / "masks"
        overlays_dir = out_dir / "overlays"
        json_dir = out_dir / "json"
        masks_dir.mkdir(parents=True, exist_ok=True)
        overlays_dir.mkdir(parents=True, exist_ok=True)
        json_dir.mkdir(parents=True, exist_ok=True)

        frames = sorted(frame_dir.glob("*_frame_*.png"))
        for frame_path in frames:
            fname = frame_path.name
            mask_path = masks_dir / fname
            overlay_path = overlays_dir / fname
            json_path = json_dir / f"{fname[:-4]}_components.json"

            if not args.overwrite and mask_path.exists():
                continue

            gray = cv2.imread(str(frame_path), cv2.IMREAD_GRAYSCALE)
            if gray is None:
                print(f"  [skip] could not read {frame_path}")
                continue

            mask, stats = segment_vessel(gray)
            cv2.imwrite(str(mask_path), mask)
            cv2.imwrite(str(overlay_path), make_overlay(gray, mask))
            with open(json_path, "w", encoding="utf-8") as f:
                json.dump(stats, f, indent=2)

            summary["processed_frames"] += 1
            if stats["num_vessels"] > 0:
                summary["frames_with_vessels"] += 1
            summary["per_frame"].append(
                {
                    "frame": fname,
                    "num_vessels": stats["num_vessels"],
                    "total_vessel_area_px": stats["total_vessel_area_px"],
                }
            )

    summary_path = output_root / "summary.json"
    with open(summary_path, "w", encoding="utf-8") as f:
        json.dump(summary, f, indent=2)

    print(f"Processed {summary['processed_frames']} frames "
          f"({summary['frames_with_vessels']} with vessels)")
    print(f"Summary written to: {summary_path}")


if __name__ == "__main__":
    main()
