# 0_track — OptiTrack Motive 3D Marker Tracking

Streams 3D marker positions from an OptiTrack Motive system. Architecture:

```
Motive cameras ──► MotiveAPI.dll (OptiTrack SDK, Qt-based)
                        ▲
                 C++ wrapper DLL (this folder /C++_wrapper)
                        ▲ plain C ABI
                 Python ctypes client (/motive_client.py)
                        ▲
                 /track_markers.py  (console + CSV output)
```

## Files

| File | Purpose |
|---|---|
| `C++_wrapper/motive_wrapper.h` | C ABI header exported by the wrapper DLL |
| `C++_wrapper/motive_wrapper.cpp` | Wrapper implementation around `MotiveAPI.*` |
| `C++_wrapper/build_wrapper.bat` | Builds `bin\motive_wrapper.dll` (x64, MSVC) |
| `C++_wrapper/test_init.cpp` | Optional standalone C++ diagnostic for `Initialize()` |
| `motive_client.py` | Python ctypes bindings (`MotiveClient`, `Marker`, `RigidBody`) |
| `track_markers.py` | Tracking script: live print + CSV logging |
| `bin/motive_wrapper.dll` | Built wrapper (rebuild with the .bat) |

## Build

```bat
cd 0_track\C++_wrapper
build_wrapper.bat
```

Requires Visual Studio Build Tools (x64 C++ workload). The Motive SDK is
picked up from `C:\Program Files\OptiTrack\Motive` by default (pass a different
install path as the first argument if needed). Output goes to `0_track\bin`.

## Run

```bat
:: live print of all markers, Ctrl+C to stop
python 0_track\track_markers.py

:: run 10 s and log every marker frame to CSV
python 0_track\track_markers.py --duration 10 --csv out\marker_log.csv

:: only follow one rigid body (e.g. an ultrasound probe)
python 0_track\track_markers.py --rigid-body "Probe"
```

Quick smoke test (loads DLL, checks licensing/devices):

```bat
python 0_track\motive_client.py
```

## Using it from other code

```python
from motive_client import MotiveClient

with MotiveClient() as client:          # initializes the API
    client.load_calibration(r"C:\ProgramData\OptiTrack\Motive\System Calibration.mcal")
    client.flush_camera_queues()
    while True:
        if client.update_single_frame():
            for m in client.get_markers():
                print(m.x, m.y, m.z)    # meters, Motive world coordinates
```

## Notes & troubleshooting

- **Coordinates are in METERS** in the Motive world coordinate system; rigid
  body yaw/pitch/roll are in degrees.
- **Close Motive.exe before running** — the API client needs exclusive device
  access (`CanConnectToDevices()` reports False otherwise).
- **Licensing**: `Initialize()` returns `kApiResult_InvalidLicense` (6) when no
  license is present. Activate one with
  `C:\Program Files\OptiTrack\Motive\LicenseTool\OptiTrackActivationTool.exe`
  or plug in a licensed OptiTrack device. The client raises a clear error in
  that case instead of crashing.
- **Qt plugin path**: the Python client sets `QT_QPA_PLATFORM_PLUGIN_PATH` /
  `QT_PLUGIN_PATH` automatically; without them Motive's Qt-based init aborts
  the whole process.
- Marker identity across frames: use each marker's Motive UID
  (`Marker.uid`) — it is stable for labeled markers. Unlabeled free markers are
  indexed by position only.
- The wrapper exposes rigid bodies too (`client.get_rigid_bodies()`) so a probe
  can be tracked as a rigid body instead of individual markers.
