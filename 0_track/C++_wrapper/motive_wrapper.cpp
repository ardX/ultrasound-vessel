//======================================================================================================
// motive_wrapper.cpp
// Implementation of the C ABI wrapper around the OptiTrack Motive API 2.x.
//
// Build (x64, MSVC): see build_wrapper.bat in this folder.
//   - /DMOTIVE_API_IMPORTS  : import symbols from MotiveAPI.dll
//   - /DMotive_wrapper_EXPORTS : export our own C functions
//======================================================================================================

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

#include "MotiveAPI.h" // provided by the OptiTrack Motive installation (inc folder)

#include "motive_wrapper.h"

namespace {

/// Convert a wide string to UTF-8 without relying on deprecated std::wstring_convert.
std::string WideToUtf8(const wchar_t* wstr)
{
    if (wstr == nullptr)
        return std::string();

    int wlen = (int) wcslen(wstr);
    int need = WideCharToMultiByte( CP_UTF8, 0, wstr, wlen, nullptr, 0, nullptr, nullptr );
    if (need <= 0)
        return std::string();

    std::string out;
    out.resize((size_t) need);
    WideCharToMultiByte( CP_UTF8, 0, wstr, wlen, &out[0], need, nullptr, nullptr );
    return out;
}

} // namespace

extern "C" {

//--- Lifecycle --------------------------------------------------------------------------------------

MW_API int MW_Initialize(void)
{
    return (int) MotiveAPI::Initialize();
}

MW_API void MW_Shutdown(void)
{
    if (MotiveAPI::IsInitialized())
        MotiveAPI::Shutdown();
}

MW_API int MW_IsInitialized(void)
{
    return MotiveAPI::IsInitialized() ? 1 : 0;
}

MW_API int MW_CanConnectToDevices(void)
{
    return MotiveAPI::CanConnectToDevices() ? 1 : 0;
}

//--- Configuration ----------------------------------------------------------------------------------

MW_API int MW_LoadCalibration(const wchar_t* filename)
{
    if (filename == nullptr)
        return (int) MotiveAPI::kApiResult_InvalidFile;
    return (int) MotiveAPI::LoadCalibration(filename);
}

MW_API int MW_LoadProfile(const wchar_t* filename)
{
    if (filename == nullptr)
        return (int) MotiveAPI::kApiResult_InvalidFile;
    return (int) MotiveAPI::LoadProfile(filename);
}

//--- Frame processing ------------------------------------------------------------------------------

MW_API void MW_FlushCameraQueues(void)
{
    MotiveAPI::FlushCameraQueues();
}

MW_API int MW_UpdateSingleFrame(void)
{
    return (int) MotiveAPI::UpdateSingleFrame();
}

MW_API int MW_FrameID(void)
{
    return MotiveAPI::FrameID();
}

MW_API double MW_FrameTimeStamp(void)
{
    return MotiveAPI::FrameTimeStamp();
}

//--- Markers ----------------------------------------------------------------------------------------

MW_API int MW_MarkerCount(void)
{
    return MotiveAPI::MarkerCount();
}

MW_API int MW_MarkerXYZ(int markerIndex, float* x, float* y, float* z)
{
    if (x != nullptr) *x = 0.0f;
    if (y != nullptr) *y = 0.0f;
    if (z != nullptr) *z = 0.0f;
    return MotiveAPI::MarkerXYZ(markerIndex, *x, *y, *z) ? 1 : 0;
}

MW_API float MW_MarkerResidual(int markerIndex)
{
    return MotiveAPI::MarkerResidual(markerIndex);
}

MW_API int MW_MarkerID(int markerIndex,
                       unsigned long long* highBits,
                       unsigned long long* lowBits)
{
    // MarkerID() is implemented inside MotiveAPI.dll and returns a cUID by value;
    // we only copy its bits out, so no Core symbols are needed at link time.
    Core::cUID id = MotiveAPI::MarkerID(markerIndex);
    if (highBits != nullptr) *highBits = id.HighBits();
    if (lowBits != nullptr)  *lowBits  = id.LowBits();

    // An invalid index yields the kInvalid UID (0,0).
    return (id.HighBits() != 0 || id.LowBits() != 0) ? 1 : 0;
}

//--- Rigid bodies -----------------------------------------------------------------------------------

MW_API int MW_RigidBodyCount(void)
{
    return MotiveAPI::RigidBodyCount();
}

MW_API int MW_RigidBodyName(int rbIndex, char* buffer, int bufferSize)
{
    if (buffer == nullptr || bufferSize <= 0)
        return 0;

    wchar_t wname[512] = { 0 };
    if (!MotiveAPI::RigidBodyName(rbIndex, wname, 512))
    {
        buffer[0] = '\0';
        return 0;
    }

    std::string utf8 = WideToUtf8(wname);
    int copyLen = (int) utf8.size();
    if (copyLen >= bufferSize)
        copyLen = bufferSize - 1;
    memcpy(buffer, utf8.data(), (size_t) copyLen);
    buffer[copyLen] = '\0';
    return 1;
}

MW_API int MW_IsRigidBodyTracked(int rbIndex)
{
    return MotiveAPI::IsRigidBodyTracked(rbIndex) ? 1 : 0;
}

MW_API int MW_RigidBodyTransform(int rbIndex,
                                 float* x, float* y, float* z,
                                 float* qx, float* qy, float* qz, float* qw,
                                 float* yaw, float* pitch, float* roll)
{
    // Default outputs so partial reads (NULL pointers) still leave sane values.
    if (x != nullptr) *x = 0.0f;
    if (y != nullptr) *y = 0.0f;
    if (z != nullptr) *z = 0.0f;
    if (qx != nullptr) *qx = 0.0f;
    if (qy != nullptr) *qy = 0.0f;
    if (qz != nullptr) *qz = 0.0f;
    if (qw != nullptr) *qw = 1.0f;
    if (yaw != nullptr)   *yaw = 0.0f;
    if (pitch != nullptr) *pitch = 0.0f;
    if (roll != nullptr)  *roll = 0.0f;

    return MotiveAPI::RigidBodyTransform(rbIndex, x, y, z, qx, qy, qz, qw, yaw, pitch, roll) ? 1 : 0;
}

} // extern "C"
