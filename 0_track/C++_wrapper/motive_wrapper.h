//======================================================================================================
// motive_wrapper.h
// C ABI wrapper around the OptiTrack Motive API 2.x (MotiveAPI.dll).
//
// This DLL exposes a plain C interface so it can be consumed from Python via ctypes.
// All lengths/coordinates follow the Motive API conventions:
//   - positions are in METERS
//   - yaw/pitch/roll are in DEGREES
//======================================================================================================
#pragma once

#ifdef _WIN32
#  ifdef MOTIVE_WRAPPER_EXPORTS
#    define MW_API __declspec(dllexport)
#  else
#    define MW_API __declspec(dllimport)
#  endif
#else
#  define MW_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

//--- Lifecycle --------------------------------------------------------------------------------------

/// Initialize the Motive API (licensing + device discovery). Must be called first.
/// @return 0 on success, otherwise the MotiveAPI eResult error code.
MW_API int MW_Initialize(void);

/// Shut down the API and release resources.
MW_API void MW_Shutdown(void);

/// @return 1 if Initialize() has been called and Shutdown() has not.
MW_API int MW_IsInitialized(void);

/// @return 1 if no other OptiTrack software (e.g. Motive.exe) is consuming the devices on this host.
MW_API int MW_CanConnectToDevices(void);

//--- Configuration ----------------------------------------------------------------------------------

/// Load a camera calibration file (.mcal). Call after Initialize().
/// @return 0 on success, otherwise the MotiveAPI eResult error code.
MW_API int MW_LoadCalibration(const wchar_t* filename);

/// Load a user profile file (.motive). Call after Initialize().
/// @return 0 on success, otherwise the MotiveAPI eResult error code.
MW_API int MW_LoadProfile(const wchar_t* filename);

//--- Frame processing ------------------------------------------------------------------------------

/// Drop all queued camera frames (call once before starting to read fresh data).
MW_API void MW_FlushCameraQueues(void);

/// Process one new frame of camera data, leaving any remaining frames queued.
/// @return 0 if a frame was processed, otherwise the MotiveAPI eResult code
///         (6 == kApiResult_NoFrameAvailable).
MW_API int MW_UpdateSingleFrame(void);

/// @return The ID of the current frame.
MW_API int MW_FrameID(void);

/// @return Timestamp of the current frame in seconds since startup.
MW_API double MW_FrameTimeStamp(void);

//--- Markers ----------------------------------------------------------------------------------------

/// @return Number of reconstructed markers in the current frame.
MW_API int MW_MarkerCount(void);

/// Retrieve the 3D reconstructed position of a marker (in meters).
/// @return 1 on success, 0 if the index is invalid.
MW_API int MW_MarkerXYZ(int markerIndex, float* x, float* y, float* z);

/// @return Reconstruction residual of the marker in mm/ray (0 if invalid index).
MW_API float MW_MarkerResidual(int markerIndex);

/// Retrieve the unique ID (128-bit UID) of a marker in the current frame.
/// Motive assigns this ID during reconstruction; it is stable across frames for
/// the same labeled marker, making it a good key for tracking in Python.
/// @return 1 on success, 0 if the index is invalid. Any output pointer may be NULL.
MW_API int MW_MarkerID(int markerIndex,
                       unsigned long long* highBits,
                       unsigned long long* lowBits);

//--- Rigid bodies -----------------------------------------------------------------------------------

/// @return Total number of defined rigid bodies (tracked or not).
MW_API int MW_RigidBodyCount(void);

/// Retrieve the name of a rigid body as UTF-8.
/// @return 1 on success, 0 otherwise (buffer is zero-terminated either way).
MW_API int MW_RigidBodyName(int rbIndex, char* buffer, int bufferSize);

/// @return 1 if the rigid body is tracked in the current frame.
MW_API int MW_IsRigidBodyTracked(int rbIndex);

/// Retrieve the world-space transform of a rigid body in the current frame.
/// Position in meters, unit quaternion (x,y,z,w), yaw/pitch/roll in degrees.
/// Any output pointer may be NULL to skip that value.
/// @return 1 if the rigid body is found and tracked, 0 otherwise.
MW_API int MW_RigidBodyTransform(int rbIndex,
                                 float* x, float* y, float* z,
                                 float* qx, float* qy, float* qz, float* qw,
                                 float* yaw, float* pitch, float* roll);

#ifdef __cplusplus
} // extern "C"
#endif
