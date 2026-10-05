"""track_markers.py

Stream 3D marker positions from an OptiTrack Motive system using the C++
wrapper DLL (built in 0_track/C++_wrapper) via ctypes.

Examples
--------
# Live print of all markers, Ctrl+C to stop:
    python track_markers.py

# Run for 10 seconds and log every marker frame to CSV:
    python track_markers.py --duration 10 --csv out/marker_log.csv

# Print only a specific rigid body's transform (e.g. an ultrasound probe):
    python track_markers.py --rigid-body "Probe"

# Print ALL rigid bodies' transforms every frame (and log them to CSV with --csv):
    python track_markers.py --all-rigid-bodies

# Discover which rigid bodies are defined in the loaded profile:
    python track_markers.py --list-rigid-bodies

Notes
-----
- Coordinates are in METERS in the Motive world coordinate system.
- Close Motive.exe before running: the API client needs exclusive device access.
  (Motive.exe and this client cannot both own the cameras at the same time.)
"""

from __future__ import annotations

import argparse
import csv
import sys
import time
from pathlib import Path
from typing import List, Optional

from motive_client import MotiveClient, Marker, RigidBody

# Default calibration/profile files written by Motive on this machine.
DEFAULT_CALIBRATION = r"C:\ProgramData\OptiTrack\Motive\System Calibration.mcal"
DEFAULT_PROFILE = r"C:\ProgramData\OptiTrack\Motive\MotiveProfile.motive"


def parse_args(argv: Optional[List[str]] = None) -> argparse.Namespace:
    p = argparse.ArgumentParser(description="OptiTrack Motive 3D marker tracking")
    p.add_argument("--duration", type=float, default=None,
                   help="Run for N seconds and exit (default: run until Ctrl+C)")
    p.add_argument("--csv", type=str, default=None,
                   help="Write a CSV log of all marker positions to this file")
    p.add_argument("--calibration", type=str, default=DEFAULT_CALIBRATION,
                   help="Path to the .mcal calibration file")
    p.add_argument("--profile", type=str, default=DEFAULT_PROFILE,
                   help="Path to the .motive profile file")
    p.add_argument("--rigid-body", type=str, default=None,
                   help="Only print this rigid body's transform (name match)")
    p.add_argument("--all-rigid-bodies", action="store_true",
                   help="Print every rigid body's transform each frame "
                        "(also logged to CSV when --csv is used)")
    p.add_argument("--list-rigid-bodies", action="store_true",
                   help="List all rigid bodies defined in the profile, then exit")
    p.add_argument("--quiet", action="store_true",
                   help="Suppress per-frame console output")
    return p.parse_args(argv)


def format_marker_line(marker: Marker) -> str:
    return (f"  marker[{marker.index}] uid=({marker.uid_high},{marker.uid_low}) "
            f"x={marker.x:+.4f}  y={marker.y:+.4f}  z={marker.z:+.4f} m")


def format_rigid_body_line(rb: RigidBody) -> str:
    if not rb.tracked:
        return f"  rigid body '{rb.name}': not tracked"
    return (f"  rigid body '{rb.name}': "
            f"x={rb.x:+.4f} y={rb.y:+.4f} z={rb.z:+.4f} m  "
            f"q=({rb.qx:+.4f},{rb.qy:+.4f},{rb.qz:+.4f},{rb.qw:+.4f})  "
            f"yaw={rb.yaw:.1f} pitch={rb.pitch:.1f} roll={rb.roll:.1f} deg")


def main(argv: Optional[List[str]] = None) -> int:
    args = parse_args(argv)

    csv_writer = None
    csv_file = None
    rb_writer = None
    rb_file = None
    if args.csv:
        Path(args.csv).parent.mkdir(parents=True, exist_ok=True)
        csv_file = open(args.csv, "w", newline="", encoding="utf-8")
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(["frame_id", "timestamp_s", "marker_index",
                             "uid_high", "uid_low", "x_m", "y_m", "z_m", "residual_mm_ray"])

        # Companion CSV for rigid body poses when rigid body data is requested.
        if args.all_rigid_bodies or args.rigid_body:
            rb_path = Path(args.csv).with_name(Path(args.csv).stem + "_rigid_bodies.csv")
            rb_file = open(rb_path, "w", newline="", encoding="utf-8")
            rb_writer = csv.writer(rb_file)
            rb_writer.writerow(["frame_id", "timestamp_s", "name", "tracked",
                                "x_m", "y_m", "z_m", "qx", "qy", "qz", "qw",
                                "yaw_deg", "pitch_deg", "roll_deg"])
            print(f"Rigid body poses will be logged to: {rb_path}")

    print("Starting OptiTrack Motive tracking client ...")
    try:
        with MotiveClient() as client:
            if not client.can_connect_to_devices():
                print("WARNING: another OptiTrack application appears to own the "
                      "devices (is Motive.exe running?). Close it and retry.")

            # Load calibration/profile exactly as the Motive samples do.
            for label, path in (("calibration", args.calibration), ("profile", args.profile)):
                if Path(path).exists():
                    getattr(client, f"load_{label}")(path)
                    print(f"Loaded {label}: {path}")
                else:
                    print(f"NOTE: {label} file not found ({path}); using API defaults.")

            # Optional: just list defined rigid bodies (from the profile) and exit.
            if args.list_rigid_bodies:
                bodies = client.get_rigid_bodies()
                if not bodies:
                    print("No rigid bodies defined. Create them in Motive "
                          "(Assets > Rigid Body) and save the profile.")
                else:
                    print(f"{len(bodies)} rigid bodies defined:")
                    for rb in bodies:
                        state = "tracked" if rb.tracked else "not tracked"
                        print(f"  [{rb.index}] {rb.name!r} ({state})")
                return 0

            client.flush_camera_queues()

            start = time.time()
            frames_received = 0
            last_print = 0.0

            while True:
                if args.duration is not None and (time.time() - start) >= args.duration:
                    break

                # Block until a new frame arrives (UpdateSingleFrame returns when
                # one has been processed; the Motive API polls internally).
                if client.update_single_frame():
                    frames_received += 1
                    frame_id = client.frame_id()
                    t = client.frame_timestamp()

                    markers: List[Marker] = client.get_markers()
                    bodies: List[RigidBody] = (
                        client.get_rigid_bodies()
                        if (args.rigid_body or args.all_rigid_bodies) else []
                    )

                    # CSV log of every marker in this frame.
                    if csv_writer is not None:
                        for m in markers:
                            csv_writer.writerow(
                                [frame_id, f"{t:.6f}", m.index,
                                 m.uid_high, m.uid_low,
                                 f"{m.x:.6f}", f"{m.y:.6f}", f"{m.z:.6f}",
                                 f"{m.residual:.3f}"])

                    # CSV log of rigid body poses in this frame.
                    if rb_writer is not None:
                        for rb in bodies:
                            rb_writer.writerow(
                                [frame_id, f"{t:.6f}", rb.name, rb.tracked,
                                 f"{rb.x:.6f}", f"{rb.y:.6f}", f"{rb.z:.6f}",
                                 f"{rb.qx:.6f}", f"{rb.qy:.6f}",
                                 f"{rb.qz:.6f}", f"{rb.qw:.6f}",
                                 f"{rb.yaw:.3f}", f"{rb.pitch:.3f}", f"{rb.roll:.3f}"])

                    # Console output (throttled to ~10 Hz).
                    if not args.quiet and (time.time() - last_print) > 0.1:
                        last_print = time.time()
                        print(f"frame {frame_id}  t={t:.3f}s  markers={len(markers)}")
                        for m in markers:
                            print(format_marker_line(m))
                        for rb in bodies:
                            if args.all_rigid_bodies:
                                print(format_rigid_body_line(rb))
                            elif args.rigid_body and rb.name.lower() == args.rigid_body.lower():
                                print(format_rigid_body_line(rb))

                # Small sleep so we don't spin at 100% CPU when no frame is pending.
                time.sleep(0.002)

            print(f"\nStopped after {frames_received} frames.")
    except RuntimeError as e:
        print(f"\nERROR: {e}")
        return 1
    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    finally:
        if csv_file is not None:
            csv_file.close()
            print(f"CSV log written to: {args.csv}")
        if rb_file is not None:
            rb_file.close()
            rb_path = Path(args.csv).with_name(Path(args.csv).stem + "_rigid_bodies.csv")
            print(f"Rigid body CSV log written to: {rb_path}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
