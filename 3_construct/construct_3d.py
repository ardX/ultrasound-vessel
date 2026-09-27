"""Build an interactive 3D representation of the vessel segmentation results.

The 2_segment step produces one binary mask per ultrasound frame:
    2_segment/output/<video_stem>/masks/*_frame_*.png

This script stacks those masks into a 3D volume and renders the vessels as a
surface mesh in an interactive PyVista (VTK) window.

Coordinate system / physical scale
----------------------------------
* Z axis = scan direction (frame index). The probe was scanned along a phantom
  that is ``--length-cm`` (default 17) cm long, so the frame-to-frame spacing
  is ``length_cm / (n_frames - 1)``.
* X / Y axes = in-plane image axes. The scanner's in-plane field of view is
  40 mm, so ``--fov-cm`` defaults to 4 across the full image width. Adjust it
  if you change probes/scanner settings.

Mouse controls (PyVista defaults)
---------------------------------
* Left drag   : rotate / orbit the object
* Right drag  : pan
* Wheel       : zoom in / out
* Middle drag : dolly
* Key "r"     : toggle auto-spin around the vertical axis
* Key "s"     : save a screenshot to 3_construct/output/<stem>_view.png
* Key "q"     : quit

Usage:
    python 3_construct/construct_3d.py
    python 3_construct/construct_3d.py --video-stem ultrasound_20260924_152247
    python 3_construct/construct_3d.py --length-cm 17 --fov-cm 4
    python 3_construct/construct_3d.py --screenshot out.png --export-mesh vessels.obj
"""

import argparse
import time
from pathlib import Path

import cv2
import numpy as np
import pyvista as pv

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SEGMENT_OUTPUT_DIR = PROJECT_ROOT / "2_segment" / "output"
CONSTRUCT_OUTPUT_DIR = PROJECT_ROOT / "3_construct" / "output"

VESSEL_COLOR = "#ff4136"       # red-ish, matches the 2_segment overlays
BOX_COLOR = "#8899aa"
SPIN_DEG_PER_SEC = 30.0        # auto-spin angular speed around the Z axis


def discover_stems(segment_dir: Path) -> list[str]:
    """Video stems that have a masks/ directory under 2_segment/output."""
    if not segment_dir.is_dir():
        return []
    return sorted(
        d.name for d in segment_dir.iterdir()
        if d.is_dir() and (d / "masks").is_dir() and list((d / "masks").glob("*_frame_*.png"))
    )


def load_mask_stack(masks_dir: Path) -> np.ndarray:
    """Load all *_frame_*.png masks in frame order -> (n_frames, h, w) uint8 {0,255}."""
    paths = sorted(
        masks_dir.glob("*_frame_*.png"),
        key=lambda p: int(p.stem.rsplit("_", 1)[-1]),
    )
    if not paths:
        raise SystemExit(f"No *_frame_*.png masks found in {masks_dir}")

    stack = np.empty((len(paths), 0, 0), dtype=np.uint8)
    for i, p in enumerate(paths):
        img = cv2.imread(str(p), cv2.IMREAD_GRAYSCALE)
        if img is None:
            raise SystemExit(f"Could not read mask: {p}")
        if i == 0:
            stack = np.empty((len(paths), img.shape[0], img.shape[1]), dtype=np.uint8)
        elif img.shape != stack.shape[1:]:
            # Resize to the first frame's shape (should not happen in practice).
            img = cv2.resize(img, (stack.shape[2], stack.shape[1]), interpolation=cv2.INTER_NEAREST)
        stack[i] = (img > 0).astype(np.uint8) * 255
    return stack


def build_volume(stack: np.ndarray, length_cm: float, fov_cm: float):
    """Return (pyvista ImageData in cm, surface mesh of the vessels)."""
    n_frames, h, w = stack.shape

    dz = length_cm / max(n_frames - 1, 1)   # scan direction spacing (cm/frame)
    dx = fov_cm / max(w - 1, 1)             # in-plane x spacing (cm/px)
    dy = fov_cm / max(h - 1, 1)             # in-plane y spacing (cm/px)

    # Voxel-based grid: one point per mask pixel; total extent = w*dx etc.
    volume = pv.ImageData(dimensions=(w, h, n_frames))
    volume.spacing = (dx, dy, dz)
    # Center the object at the origin so mouse rotation feels natural.
    volume.origin = (-(w - 1) * dx / 2.0, -(h - 1) * dy / 2.0, 0.0)
    volume.point_data["vessel"] = stack.astype(np.float32).ravel()
    volume.set_active_scalars("vessel")

    # Marching cubes at iso-value 127.5 (between the 0 and 255 voxel values).
    # NOTE: extract_surface() on an ImageData ignores scalars and returns the
    # full dataset bounding box, so contour() with an explicit value is used.
    surface = volume.contour(127.5)
    if surface.n_points == 0:
        raise SystemExit("No vessel voxels found in the mask stack - nothing to render.")

    # Color the surface by scan position (z) so you can see where along the
    # 17 cm phantom each vessel cross-section was captured.
    z = surface.points[:, 2]
    surface["scan_pos_cm"] = z
    return volume, surface, (dx, dy, dz)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Interactive 3D visualization of the 2_segment vessel masks."
    )
    parser.add_argument("--video-stem", default=None,
                        help="video stem under 2_segment/output (default: auto-detect)")
    parser.add_argument("--masks-dir", default=None,
                        help="explicit path to a masks/ directory (overrides --video-stem)")
    parser.add_argument("--length-cm", type=float, default=17.0,
                        help="phantom length along the scan direction in cm (default: 17)")
    parser.add_argument("--fov-cm", type=float, default=4.0,
                        help="in-plane field of view across image width in cm (scanner FOV; default: 4 = 40 mm)")
    parser.add_argument("--solid", action="store_true",
                        help="single vessel color instead of scan-position coloring")
    parser.add_argument("--no-box", action="store_true",
                        help="do not show the phantom extent bounding box")
    parser.add_argument("--screenshot", default=None,
                        help="save a PNG screenshot to this path and exit (no window)")
    parser.add_argument("--export-mesh", default=None,
                        help="export the vessel surface mesh (.obj/.stl/.ply/.vtk) to this path")
    args = parser.parse_args()

    # --- Locate masks -----------------------------------------------------
    if args.masks_dir:
        masks_dir = Path(args.masks_dir)
        stem = masks_dir.parent.name
    else:
        stems = discover_stems(SEGMENT_OUTPUT_DIR)
        if not stems:
            raise SystemExit(
                f"No segmentation results found under {SEGMENT_OUTPUT_DIR}. "
                "Run 2_segment/segment_vessels.py first."
            )
        if args.video_stem:
            if args.video_stem not in stems:
                raise SystemExit(f"'{args.video_stem}' not found. Available: {', '.join(stems)}")
            stem = args.video_stem
        elif len(stems) == 1:
            stem = stems[0]
        else:
            raise SystemExit(
                f"Multiple videos found ({', '.join(stems)}); pass --video-stem <name>."
            )
        masks_dir = SEGMENT_OUTPUT_DIR / stem / "masks"

    # --- Build volume -----------------------------------------------------
    print(f"Loading masks for '{stem}' from {masks_dir} ...")
    stack = load_mask_stack(masks_dir)
    n_frames, h, w = stack.shape
    volume, surface, (dx, dy, dz) = build_volume(stack, args.length_cm, args.fov_cm)

    n_voxels = int((stack > 0).sum())
    print(f"Frames: {n_frames} | image: {w}x{h} px | vessel voxels: {n_voxels}")
    print(f"Spacing (cm): x={dx:.4f} y={dy:.4f} z={dz:.4f} "
          f"| extent: {(w - 1) * dx:.1f} x {(h - 1) * dy:.1f} x {args.length_cm:.1f} cm")

    if args.export_mesh:
        out = Path(args.export_mesh)
        out.parent.mkdir(parents=True, exist_ok=True)
        surface.save(str(out))
        print(f"Mesh exported to: {out}")

    # --- Plotter ----------------------------------------------------------
    title = (f"{stem}  |  {n_frames} frames over {args.length_cm:.0f} cm  |  "
             f"FOV {args.fov_cm * 10:.0f} mm  |  axes in cm, Z = scan direction")

    plotter = pv.Plotter(window_size=(1280, 800))
    plotter.set_background("#101418")

    if args.solid:
        plotter.add_mesh(surface, color=VESSEL_COLOR, opacity=1.0,
                         show_edges=False, name="vessels")
    else:
        plotter.add_mesh(
            surface, scalars="scan_pos_cm", cmap="coolwarm",
            opacity=1.0, show_edges=False, name="vessels",
            scalar_bar_args={"title": "scan position (cm)", "n_labels": 5,
                             "color": "white"},
        )

    if not args.no_box:
        box = pv.Box(bounds=volume.bounds)  # (xmin, xmax, ymin, ymax, zmin, zmax)
        plotter.add_mesh(box, color=BOX_COLOR, opacity=0.08, name="phantom")
        plotter.add_mesh(box.outline(), color="#cfd8dc", line_width=1.5,
                         name="phantom_outline")

    # White axis labels/ticks for the dark background.
    plotter.add_axes(color="white", xlabel="X (cm)", ylabel="Y (cm)",
                     zlabel="Z - scan (cm)")

    # Key bindings: r = toggle auto-spin, s = screenshot, q handled by default.
    state = {"screenshot_dir": CONSTRUCT_OUTPUT_DIR, "spin": True, "_last": None}

    def spin_step():
        """Orbit the camera around the Z (scan) axis - the 'spin' control.

        Time-based (30 deg/s) so the speed is independent of frame rate.
        """
        if not state["spin"]:
            return
        now = time.time()
        dt = min(now - state["_last"], 0.1) if state["_last"] else 0.0
        state["_last"] = now
        cam = plotter.camera
        center = np.asarray(plotter.renderer.center, dtype=float)
        inc = SPIN_DEG_PER_SEC * dt * np.pi / 180.0
        dx, dy, dz = cam.position - center
        ca, sa = np.cos(inc), np.sin(inc)
        # Rotate the camera position around the z axis through the center.
        # (All three components must be re-anchored on `center` - forgetting
        # center[2] makes the camera drift away by center[2] per frame.)
        cam.position = (center[0] + dx * ca - dy * sa,
                        center[1] + dx * sa + dy * ca,
                        center[2] + dz)

    def toggle_rotation(obj=None):
        state["spin"] = not state["spin"]
        print(f"Auto-spin: {'ON' if state['spin'] else 'OFF'}")

    def take_screenshot(obj=None):
        state["screenshot_dir"].mkdir(parents=True, exist_ok=True)
        path = state["screenshot_dir"] / f"{stem}_view.png"
        plotter.screenshot(str(path))
        print(f"Screenshot saved: {path}")

    plotter.add_key_event("r", toggle_rotation)
    plotter.add_key_event("s", take_screenshot)

    if args.screenshot:
        out = Path(args.screenshot)
        out.parent.mkdir(parents=True, exist_ok=True)
        plotter.show(title=title, interactive=False, auto_close=False, screenshot=str(out))
        print(f"Screenshot saved: {out}")
        return

    # Start auto-spinning by default (the "spin" control); press r to stop.
    # NOTE: in this pyvista/VTK build, VTK repeating timers and RenderEvent
    # observers do NOT fire inside the interactive event loop (verified), so
    # the spin is driven from a Python-level hook: wrap render_window.Render,
    # which pyvista's own event loop calls once per frame.
    _orig_render = plotter.render_window.Render

    def _spin_render():
        spin_step()
        _orig_render()

    plotter.render_window.Render = _spin_render
    print("Mouse: left-drag rotate | right-drag pan | wheel zoom | 'r' toggle spin | 's' screenshot")
    plotter.show(title=title)


if __name__ == "__main__":
    main()
