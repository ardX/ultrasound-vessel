"""Extract all frames from a video and save video metadata as JSON.

Usage:
    python data/extract_frames.py                          # uses default input/output
    python data/extract_frames.py <input_video>            # custom input video
    python data/extract_frames.py <input_video> <output_dir>

Default input : data/video/ultrasound_20260924_152247.mp4
Default output: data/video2images/<video_stem>/
Metadata JSON : data/video2images/<video_stem>/<video_stem>_metadata.json
"""

import json
import sys
from pathlib import Path

import cv2

# ---------------------------------------------------------------------------
# Paths (relative to the project root, i.e. ULTRASOUND/)
# ---------------------------------------------------------------------------
PROJECT_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_INPUT = PROJECT_ROOT / "data" / "video" / "ultrasound_20260924_152247.mp4"
DEFAULT_OUTPUT_DIR = PROJECT_ROOT / "data" / "video2images"


def get_metadata(video_path: Path) -> dict:
    """Read video metadata with OpenCV and build a JSON-serialisable dict."""
    cap = cv2.VideoCapture(str(video_path))
    if not cap.isOpened():
        raise RuntimeError(f"Could not open video: {video_path}")

    fps = cap.get(cv2.CAP_PROP_FPS)
    frame_count = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    # Duration: prefer the container-reported value, fall back to fps estimate.
    duration_ms = cap.get(cv2.CAP_PROP_POS_MSEC)  # unreliable in some builds
    if not duration_ms or duration_ms <= 0:
        duration_ms = (frame_count / fps * 1000.0) if fps > 0 else 0.0

    cap.release()

    return {
        "source_file": video_path.name,
        "source_path": str(video_path),
        "format": video_path.suffix.lower().lstrip("."),
        "width_px": width,
        "height_px": height,
        "fps": round(fps, 3) if fps > 0 else None,
        "frame_count": frame_count,
        "duration_seconds": round(duration_ms / 1000.0, 3),
        "duration_ms": round(duration_ms, 1),
        "file_size_bytes": video_path.stat().st_size,
        "file_size_mb": round(video_path.stat().st_size / (1024 * 1024), 2),
    }


def extract_frames(video_path: Path, output_dir: Path) -> int:
    """Extract every frame as a PNG image. Returns the number of frames saved."""
    output_dir.mkdir(parents=True, exist_ok=True)

    cap = cv2.VideoCapture(str(video_path))
    if not cap.isOpened():
        raise RuntimeError(f"Could not open video: {video_path}")

    stem = video_path.stem
    count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        out_path = output_dir / f"{stem}_frame_{count:06d}.png"
        cv2.imwrite(str(out_path), frame)
        count += 1

    cap.release()
    return count


def main() -> None:
    # ------------------------------------------------------------------
    # Parse arguments
    # ------------------------------------------------------------------
    input_path = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_INPUT
    output_dir = (
        Path(sys.argv[2])
        if len(sys.argv) > 2
        else DEFAULT_OUTPUT_DIR / input_path.stem
    )

    if not input_path.is_file():
        raise SystemExit(f"Input video not found: {input_path}")

    print(f"Input : {input_path}")
    print(f"Output: {output_dir}")

    # ------------------------------------------------------------------
    # Extract frames
    # ------------------------------------------------------------------
    saved = extract_frames(input_path, output_dir)
    print(f"Extracted {saved} frames to: {output_dir}")

    # ------------------------------------------------------------------
    # Save metadata JSON (use the actually-saved frame count as ground truth)
    # ------------------------------------------------------------------
    meta = get_metadata(input_path)
    meta["extracted_frames"] = saved
    meta_json_path = output_dir / f"{input_path.stem}_metadata.json"
    with open(meta_json_path, "w", encoding="utf-8") as f:
        json.dump(meta, f, indent=2, ensure_ascii=False)
    print(f"Metadata saved to: {meta_json_path}")


if __name__ == "__main__":
    main()
