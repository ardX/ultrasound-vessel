r"""motive_client.py

Python bindings (via ctypes) for the C++ wrapper DLL around the OptiTrack
Motive API 2.x. The wrapper exposes a plain C ABI so Python can call it
directly without SWIG/ctypes header parsing.

Usage:
    from motive_client import MotiveClient

    with MotiveClient() as client:
        client.load_calibration(r"C:\ProgramData\OptiTrack\Motive\System Calibration.mcal")
        client.flush_camera_queues()
        while True:
            if client.update_single_frame():
                for m in client.get_markers():
                    print(m.x, m.y, m.z)

Coordinates are in METERS (Motive world coordinate system).
"""

from __future__ import annotations

import ctypes
import os
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional, Tuple

# ---------------------------------------------------------------------------
# DLL locations
#   - motive_wrapper.dll : 0_track/bin (created by C++_wrapper/build_wrapper.bat)
#   - MotiveAPI.dll      : copied to 0_track/bin, but its own dependencies
#                          (Qt5*, Poco*, opencv_world, ...) live in the Motive
#                          install directory, which must be on the DLL path.
# ---------------------------------------------------------------------------
_THIS_DIR = Path(__file__).resolve().parent
_BIN_DIR = _THIS_DIR / "bin"
MOTIVE_INSTALL_DIR = Path(
    os.environ.get("MOTIVE_HOME", r"C:\Program Files\OptiTrack\Motive")
)


def _load_dll() -> ctypes.CDLL:
    """Load motive_wrapper.dll, ensuring its dependency chain can be resolved.

    Dependency chain:
        motive_wrapper.dll  (0_track/bin)
          -> MotiveAPI.dll   (Motive install /lib)
          -> Qt5*, Poco*, opencv_world454.dll, ... (Motive install root)

    Also sets Qt plugin environment variables so the Qt-based Motive API can
    find its platform plugin instead of hard-crashing during Initialize().
    """
    # Qt needs to know where its plugins live; without this, Initialize()
    # aborts the whole process ("no Qt platform plugin could be initialized").
    os.environ.setdefault("QT_QPA_PLATFORM_PLUGIN_PATH", str(MOTIVE_INSTALL_DIR / "plugins" / "platforms"))
    os.environ.setdefault("QT_PLUGIN_PATH", str(MOTIVE_INSTALL_DIR / "plugins"))
    dll_path = _BIN_DIR / "motive_wrapper.dll"
    if not dll_path.exists():
        raise FileNotFoundError(
            f"{dll_path} not found.\n"
            "Build it first:  cd 0_track\\C++_wrapper && build_wrapper.bat"
        )

    # Add the directories that hold the dependency chain, in order of priority.
    search_dirs = []
    for directory in (_BIN_DIR, MOTIVE_INSTALL_DIR / "lib", MOTIVE_INSTALL_DIR):
        if directory.is_dir():
            os.add_dll_directory(str(directory))
            search_dirs.append(str(directory))

    # Also prepend to PATH: Windows resolves *transitive* DLL dependencies of
    # MotiveAPI.dll (Qt5*, ffmpeg, opencv, ...) through the legacy PATH search,
    # which AddDllDirectory alone does not cover.
    if search_dirs:
        os.environ["PATH"] = os.pathsep.join(search_dirs) + os.pathsep + os.environ.get("PATH", "")

    return ctypes.WinDLL(str(dll_path))


# ---------------------------------------------------------------------------
# Motive API result codes (from MotiveAPI.h eResult)
# ---------------------------------------------------------------------------
RESULT_SUCCESS = 0
RESULT_FAILED = 1
RESULT_FILE_NOT_FOUND = 2
RESULT_LOAD_FAILED = 3
RESULT_SAVE_FAILED = 4
RESULT_INVALID_FILE = 5
RESULT_INVALID_LICENSE = 6
RESULT_NO_FRAME_AVAILABLE = 7

_RESULT_NAMES = {
    0: "Success",
    1: "Failed",
    2: "FileNotFound",
    3: "LoadFailed",
    4: "SaveFailed",
    5: "InvalidFile",
    6: "InvalidLicense",
    7: "NoFrameAvailable",
}


def result_name(code: int) -> str:
    return _RESULT_NAMES.get(code, f"Unknown({code})")

# Marker flag bits (Core::eMarkerFlags), for reference
FLAG_OCCLUDED = 1 << 0
FLAG_POINT_CLOUD_SOLVED = 1 << 1


# ---------------------------------------------------------------------------
# Data containers
# ---------------------------------------------------------------------------
@dataclass
class Marker:
    """A single reconstructed marker in the current frame."""
    index: int
    uid_high: int          # high 64 bits of the marker's Motive UID
    uid_low: int           # low 64 bits
    x: float               # meters
    y: float               # meters
    z: float               # meters
    residual: float        # mm/ray reconstruction residual

    @property
    def uid(self) -> Tuple[int, int]:
        return (self.uid_high, self.uid_low)

    @property
    def position(self) -> Tuple[float, float, float]:
        return (self.x, self.y, self.z)


@dataclass
class RigidBody:
    """A rigid body transform in the current frame."""
    index: int
    name: str
    tracked: bool
    x: float = 0.0
    y: float = 0.0
    z: float = 0.0
    qx: float = 0.0
    qy: float = 0.0
    qz: float = 0.0
    qw: float = 1.0
    yaw: float = 0.0       # degrees
    pitch: float = 0.0     # degrees
    roll: float = 0.0      # degrees


# ---------------------------------------------------------------------------
# Client
# ---------------------------------------------------------------------------
class MotiveClient:
    """Thin, typed wrapper around the motive_wrapper.dll C API."""

    def __init__(self, dll_path: Optional[Path] = None):
        if dll_path is not None:
            parent = str(dll_path.parent)
            os.add_dll_directory(parent)
            os.environ["PATH"] = parent + os.pathsep + os.environ.get("PATH", "")
            self._dll = ctypes.WinDLL(str(dll_path))
        else:
            self._dll = _load_dll()

        self._bind_signatures()
        self._initialized = False

    # -- signature binding -------------------------------------------------
    def _bind_signatures(self) -> None:
        d = self._dll
        c_int, c_float, c_double, c_wchar_p = (
            ctypes.c_int, ctypes.c_float, ctypes.c_double, ctypes.c_wchar_p
        )

        d.MW_Initialize.argtypes = []
        d.MW_Initialize.restype = c_int

        d.MW_Shutdown.argtypes = []
        d.MW_Shutdown.restype = None

        d.MW_IsInitialized.argtypes = []
        d.MW_IsInitialized.restype = c_int

        d.MW_CanConnectToDevices.argtypes = []
        d.MW_CanConnectToDevices.restype = c_int

        d.MW_LoadCalibration.argtypes = [c_wchar_p]
        d.MW_LoadCalibration.restype = c_int

        d.MW_LoadProfile.argtypes = [c_wchar_p]
        d.MW_LoadProfile.restype = c_int

        d.MW_FlushCameraQueues.argtypes = []
        d.MW_FlushCameraQueues.restype = None

        d.MW_UpdateSingleFrame.argtypes = []
        d.MW_UpdateSingleFrame.restype = c_int

        d.MW_FrameID.argtypes = []
        d.MW_FrameID.restype = c_int

        d.MW_FrameTimeStamp.argtypes = []
        d.MW_FrameTimeStamp.restype = c_double

        d.MW_MarkerCount.argtypes = []
        d.MW_MarkerCount.restype = c_int

        d.MW_MarkerXYZ.argtypes = [c_int, ctypes.POINTER(c_float),
                                   ctypes.POINTER(c_float), ctypes.POINTER(c_float)]
        d.MW_MarkerXYZ.restype = c_int

        d.MW_MarkerResidual.argtypes = [c_int]
        d.MW_MarkerResidual.restype = c_float

        d.MW_MarkerID.argtypes = [c_int,
                                  ctypes.POINTER(ctypes.c_ulonglong),
                                  ctypes.POINTER(ctypes.c_ulonglong)]
        d.MW_MarkerID.restype = c_int

        d.MW_RigidBodyCount.argtypes = []
        d.MW_RigidBodyCount.restype = c_int

        d.MW_RigidBodyName.argtypes = [c_int, ctypes.c_char_p, c_int]
        d.MW_RigidBodyName.restype = c_int

        d.MW_IsRigidBodyTracked.argtypes = [c_int]
        d.MW_IsRigidBodyTracked.restype = c_int

        d.MW_RigidBodyTransform.argtypes = [
            c_int,
            ctypes.POINTER(c_float), ctypes.POINTER(c_float), ctypes.POINTER(c_float),
            ctypes.POINTER(c_float), ctypes.POINTER(c_float), ctypes.POINTER(c_float), ctypes.POINTER(c_float),
            ctypes.POINTER(c_float), ctypes.POINTER(c_float), ctypes.POINTER(c_float),
        ]
        d.MW_RigidBodyTransform.restype = c_int

    # -- lifecycle ----------------------------------------------------------
    def initialize(self) -> None:
        """Initialize the Motive API (licensing + device discovery)."""
        result = self._dll.MW_Initialize()
        if result == RESULT_INVALID_LICENSE:
            raise RuntimeError(
                "Motive API is not licensed on this machine (kApiResult_InvalidLicense).\n"
                "Run OptiTrackActivationTool.exe (in the Motive install folder) to activate a\n"
                "license, or plug in your OptiTrack hardware key / licensed device."
            )
        if result != RESULT_SUCCESS:
            raise RuntimeError(f"Motive API Initialize failed: {result_name(result)}")
        self._initialized = True

    def shutdown(self) -> None:
        if self._initialized:
            self._dll.MW_Shutdown()
            self._initialized = False

    def is_initialized(self) -> bool:
        return bool(self._dll.MW_IsInitialized())

    def can_connect_to_devices(self) -> bool:
        """False when another OptiTrack app (e.g. Motive.exe) owns the devices."""
        return bool(self._dll.MW_CanConnectToDevices())

    # -- configuration --------------------------------------------------------
    def load_calibration(self, path: str) -> None:
        result = self._dll.MW_LoadCalibration(path)
        if result != RESULT_SUCCESS:
            raise RuntimeError(f"LoadCalibration({path}) failed: {result_name(result)}")

    def load_profile(self, path: str) -> None:
        result = self._dll.MW_LoadProfile(path)
        if result != RESULT_SUCCESS:
            raise RuntimeError(f"LoadProfile({path}) failed: {result_name(result)}")

    # -- frame processing -------------------------------------------------------
    def flush_camera_queues(self) -> None:
        self._dll.MW_FlushCameraQueues()

    def update_single_frame(self) -> bool:
        """Process one new camera frame. Returns True if a frame was available."""
        result = self._dll.MW_UpdateSingleFrame()
        return result == RESULT_SUCCESS

    def frame_id(self) -> int:
        return self._dll.MW_FrameID()

    def frame_timestamp(self) -> float:
        return self._dll.MW_FrameTimeStamp()

    # -- data retrieval ----------------------------------------------------------
    def marker_count(self) -> int:
        return self._dll.MW_MarkerCount()

    def get_markers(self) -> List[Marker]:
        """All reconstructed markers in the current frame (positions in meters)."""
        count = self._dll.MW_MarkerCount()
        markers: List[Marker] = []
        x = ctypes.c_float(0.0)
        y = ctypes.c_float(0.0)
        z = ctypes.c_float(0.0)
        hi = ctypes.c_ulonglong(0)
        lo = ctypes.c_ulonglong(0)

        for i in range(count):
            if not self._dll.MW_MarkerXYZ(i, ctypes.byref(x), ctypes.byref(y), ctypes.byref(z)):
                continue
            self._dll.MW_MarkerID(i, ctypes.byref(hi), ctypes.byref(lo))
            markers.append(Marker(
                index=i,
                uid_high=hi.value,
                uid_low=lo.value,
                x=float(x.value),
                y=float(y.value),
                z=float(z.value),
                residual=float(self._dll.MW_MarkerResidual(i)),
            ))
        return markers

    def get_rigid_bodies(self) -> List[RigidBody]:
        """All rigid bodies in the current frame with their world transforms."""
        count = self._dll.MW_RigidBodyCount()
        bodies: List[RigidBody] = []
        buf = ctypes.create_string_buffer(512)

        x = ctypes.c_float(0.0); y = ctypes.c_float(0.0); z = ctypes.c_float(0.0)
        qx = ctypes.c_float(0.0); qy = ctypes.c_float(0.0)
        qz = ctypes.c_float(0.0); qw = ctypes.c_float(1.0)
        yaw = ctypes.c_float(0.0); pitch = ctypes.c_float(0.0); roll = ctypes.c_float(0.0)

        for i in range(count):
            self._dll.MW_RigidBodyName(i, buf, 512)
            name = buf.value.decode("utf-8", errors="replace")
            tracked = bool(self._dll.MW_IsRigidBodyTracked(i))
            if tracked:
                self._dll.MW_RigidBodyTransform(
                    i,
                    ctypes.byref(x), ctypes.byref(y), ctypes.byref(z),
                    ctypes.byref(qx), ctypes.byref(qy), ctypes.byref(qz), ctypes.byref(qw),
                    ctypes.byref(yaw), ctypes.byref(pitch), ctypes.byref(roll),
                )
            bodies.append(RigidBody(
                index=i, name=name, tracked=tracked,
                x=float(x.value), y=float(y.value), z=float(z.value),
                qx=float(qx.value), qy=float(qy.value), qz=float(qz.value), qw=float(qw.value),
                yaw=float(yaw.value), pitch=float(pitch.value), roll=float(roll.value),
            ))
        return bodies

    # -- context manager -----------------------------------------------------
    def __enter__(self) -> "MotiveClient":
        self.initialize()
        return self

    def __exit__(self, exc_type, exc, tb) -> None:
        self.shutdown()


if __name__ == "__main__":
    # Quick smoke test: load DLL, initialize, print device availability.
    client = MotiveClient()
    print("DLL loaded:", _BIN_DIR / "motive_wrapper.dll", flush=True)
    print("Can connect to devices:", client.can_connect_to_devices(), flush=True)
    try:
        client.initialize()
        print("Motive API initialized OK.", flush=True)
        client.flush_camera_queues()
        # Try a few frames (cameras may need a moment to start streaming).
        got = 0
        for _ in range(30):
            if client.update_single_frame():
                got += 1
                markers = client.get_markers()
                print(f"frame {client.frame_id()} t={client.frame_timestamp():.3f}s "
                      f"markers={len(markers)}")
                for m in markers[:5]:
                    print(f"  [{m.index}] uid=({m.uid_high},{m.uid_low}) "
                          f"x={m.x:+.4f} y={m.y:+.4f} z={m.z:+.4f} m")
        if got == 0:
            print("No frames received (are cameras powered and calibrated?).")
    except RuntimeError as e:
        print(f"\nERROR: {e}")
    finally:
        client.shutdown()
