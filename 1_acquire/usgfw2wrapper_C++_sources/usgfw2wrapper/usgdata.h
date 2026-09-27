
#ifndef _USGDATA_H_
#define _USGDATA_H_

#include <windef.h>

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */


typedef struct _SCALEFACTOR {
	ULONG Numerator;
	ULONG Denominator;
} SCALEFACTOR, *PSCALEFACTOR;

typedef struct _EXVALUE
{
	LONG value;
	SCALEFACTOR ScaleFactor;
} EXVALUE, *PEXVALUE;

typedef struct _USGFRAMEINFOHEADER {
	DWORD size;
	DWORD PointsPerBeam;
	DWORD BeamsPerFrame;
	DWORD BitCount;
	DWORD Pitch;
	DWORD FrameSize;
	DWORD TimePerPoint;
	DWORD TimePerBeam;
} USGFRAMEINFOHEADER, *PUSGFRAMEINFOHEADER;

typedef struct _USGBEAM_VECTOR {
	POINT StartPoint;
	EXVALUE Direction;
} USGBEAM_VECTOR, *PUSGBEAM_VECTOR;

typedef struct _BULTRASOUND_INFO {
	RECT rcSource;
	RECT rcTarget;
	ULONG PointTime;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
	USGBEAM_VECTOR UsgBeamVector[1];
} BULTRASOUND_INFO, *PBULTRASOUND_INFO;

typedef struct _BULTRASOUND_INFOHEADER {
	RECT rcSource;
	RECT rcTarget;
	ULONG PointTime;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} BULTRASOUND_INFOHEADER, *PBULTRASOUND_INFOHEADER;

typedef struct _USGFRAMEINFO_BMODE {
	USGFRAMEINFOHEADER InfoHeader;
	USGBEAM_VECTOR BeamVector[1];
} USGFRAMEINFO_BMODE, *PUSGFRAMEINFO_BMODE;

typedef struct _BULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD BeamsCount;
	DWORD PointsCount;
	DWORD MinInputBeams;
	DWORD MaxInputBeams;
	DWORD MinInputPoints;
	DWORD MaxInputPoints;
	DWORD MinPointTime;
	DWORD MaxPointTime;
	int CropBeamsGranuality;
	int CropPointsGranuality;
	int CropPointTimeGranuality;
	int CropAlignBeam;
	int CropAlignPoints;
	DWORD MinOutputBeams;
	DWORD MaxOutputBeams;
	DWORD MinOutputPoints;
	DWORD MaxOutputPoints;
	int StretchTapsBeams;
	int StretchTapsPoints;
	int ShrinkTapsBeams;
	int ShrinkTapsPoints;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} BULTRASOUND_STREAM_CONFIG_CAPS, *PBULTRASOUND_STREAM_CONFIG_CAPS;

typedef struct _USGFRAMEINFO_MMODE
{
	USGFRAMEINFOHEADER InfoHeader;
	USGBEAM_VECTOR UsgBeamVector;
	DWORD FrameHeaderSize;
} USGFRAMEINFO_MMODE, *PUSGFRAMEINFO_MMODE;

typedef struct _MULTRASOUND_INFOHEADER {
	// EXVALUE FirstPointTime;
	// EXVALUE UsgBeamTime;
	LONG FirstPoint;
	LONG PointsNumber;
	DWORD BeamNumber;
	RECT rcTarget;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} MULTRASOUND_INFOHEADER, *PMULTRASOUND_INFOHEADER;

typedef struct _MULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD PointsCount;
	DWORD MinInputPoints;
	DWORD MaxInputPoints;
	DWORD MinBeamNumber;
	DWORD MaxBeamNumber;
	DWORD MinBeamTime;
	DWORD MaxBeamTime;
	DWORD MinPointTime;
	DWORD MaxPointTime;
	int CropGranualityBeamNo;
	int CropGranualityPoints;
	int CropAlignPoints;
	int CropGranualityPointTime;
	int CropGranualityBeamTime;
	SIZE MinOutputSize;
	SIZE MaxOutputSize;
	int OutputGranualityX;
	int OutputGranualityY;
	int StretchTapsX;
	int StretchTapsY;
	int ShrinkTapsX;
	int ShrinkTapsY;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} MULTRASOUND_STREAM_CONFIG_CAPS, *PMULTRASOUND_STREAM_CONFIG_CAPS;


// doppler format structures

// CFM

typedef struct _CFMULTRASOUND_INFO {
	RECT rcSource;
	RECT rcTarget;
	ULONG PointTime;
	ULONG BaseLine;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
	USGBEAM_VECTOR UsgBeamVector[1];
} CFMULTRASOUND_INFO, *PCFMULTRASOUND_INFO;

typedef struct _CFMULTRASOUND_INFOHEADER {
	RECT rcSource;
	RECT rcTarget;
	ULONG PointTime;
	ULONG BaseLine;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} CFMULTRASOUND_INFOHEADER, *PCFMULTRASOUND_INFOHEADER;

typedef struct _USGFRAMEINFO_CFMMODE {
	USGFRAMEINFOHEADER InfoHeader;
	USGBEAM_VECTOR BeamVector[1];
} USGFRAMEINFO_CFMMODE, *PUSGFRAMEINFO_CFMMODE;

typedef struct _CFMULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD BeamsCount;
	DWORD PointsCount;
	DWORD MinInputBeams;
	DWORD MaxInputBeams;
	DWORD MinInputPoints;
	DWORD MaxInputPoints;
	DWORD MinPointTime;
	DWORD MaxPointTime;
	DWORD MinBaseLine;
	DWORD MaxBaseLine;
	int CropBeamsGranuality;
	int CropPointsGranuality;
	int CropPointTimeGranuality;
	int CropBaseLineGranuality;
	int CropAlignBeam;
	int CropAlignPoints;
	int CropAlignBaseLine;
	DWORD MinOutputBeams;
	DWORD MaxOutputBeams;
	DWORD MinOutputPoints;
	DWORD MaxOutputPoints;
	int StretchTapsBeams;
	int StretchTapsPoints;
	int ShrinkTapsBeams;
	int ShrinkTapsPoints;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} CFMULTRASOUND_STREAM_CONFIG_CAPS, *PCFMULTRASOUND_STREAM_CONFIG_CAPS;


typedef struct _DATAFORMAT {
	GUID FormatID;
	SCALEFACTOR ScaleFactor;
} DATAFORMAT, *PDATAFORMAT;


typedef struct _DATAFORMAT_BOUNDS {
	GUID FormatID;
	SCALEFACTOR MinScaleFactor;
	SCALEFACTOR MaxScaleFactor;
} DATAFORMAT_BOUNDS, *PDATAFORMAT_BOUNDS;

typedef struct _VALUE_RANGE {
	LONG Stepping;
	LONG Minimum;
	LONG Maximum; 
} VALUE_RANGE, *PVALUE_RANGE;

typedef union {
	struct {
		LONG SignedMinimum;
		LONG SignedMaximum;
	};
	struct {
		ULONG UnsignedMinimum;
		ULONG UnsignedMaximum;
	};
} BOUNDS, *PBOUNDS;

typedef struct _FOCUS_DATA {
	ULONG FocusesNum;
	BOOL FocusOn[1];
} FOCUS_DATA, *PFOCUS_DATA;

typedef struct _FOCUS_DESC {
	ULONG FocusPoint;
	BOUNDS FocalZone;
} FOCUS_DESC, *PFOCUS_DESC;

typedef struct _FOCUSES_DESC {
	ULONG FocusesNum;
	FOCUS_DESC FocusDesc[1];
} FOCUSES_DESC, *PFOCUSES_DESC;

#define _FOCUS_MODE_MULTI	1
#define _FOCUS_MODE_DYNAMIC	2
#define _FOCUS_MODE_SELECTABLE	3


typedef struct
{
	LONG Frequency;
	LONG Harmonic;
} FREQUENCY_HARMONIC;

typedef struct  
{
	ULONG BatteryChargeStatus;
	ULONG PowerLineStatus;
	LONG BatteryFullLifetime;
	LONG BatteryLifePercent;
	LONG BatteryLifeRemaining;
} DEVBATTERY_STATUS;

#define POWERLINESTATUS_OFFLINE 0
#define POWERLINESTATUS_ONLINE  1
#define POWERLINESTATUS_UNKNOWN	2

#define BATCHARGESTATUS_CHARGING	1
#define BATCHARGESTATUS_CRITICAL	2
#define BATCHARGESTATUS_HIGH		4
#define BATCHARGESTATUS_LOW			8
#define BATCHARGESTATUS_NOBATTERY	0x10
#define BATCHARGESTATUS_UNKNOWN		0xff

typedef struct _ULTRASOUND2D_INFO {
	RECT rcSource;
	RECT rcTarget;
	ULONG PointTime;
	LONG ViewAngle;
	ULONG ScanningFlags;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
	USGBEAM_VECTOR UsgBeamVector[1];
} ULTRASOUND2D_INFO, *PULTRASOUND2D_INFO;

typedef struct _ULTRASOUND2D_INFOHEADER {
	RECT rcSource;
	RECT rcTarget;
	ULONG PointTime;
	LONG ViewAngle;
	ULONG ScanningFlags;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} ULTRASOUND2D_INFOHEADER, *P2DULTRASOUND2D_INFOHEADER;

typedef struct _ULTRASOUND2D_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD BeamsCount;
	DWORD PointsCount;
	DWORD ScanningCapFlags;
	LONG MinViewAngle;
	LONG MaxViewAngle;
	DWORD MinInputBeams;
	DWORD MaxInputBeams;
	DWORD MinInputPoints;
	DWORD MaxInputPoints;
	DWORD MinPointTime;
	DWORD MaxPointTime;
	int CropAngleGranularity;
	int CropBeamsGranularity;
	int CropPointsGranularity;
	int CropPointTimeGranularity;
	int CropAlignBeam;
	int CropAlignPoints;
	DWORD MinOutputBeams;
	DWORD MaxOutputBeams;
	DWORD MinOutputPoints;
	DWORD MaxOutputPoints;
	int StretchTapsBeams;
	int StretchTapsPoints;
	int ShrinkTapsBeams;
	int ShrinkTapsPoints;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} ULTRASOUND2D_STREAM_CONFIG_CAPS, *PULTRASOUND2D_STREAM_CONFIG_CAPS;

#define COMPCAPF_SingleFrameSlop	0x0001		// means compound mode can consist from one slopped subframe
#define COMPCAPF_SingleFrameDirect	0x0002		// means compound mode can to consist from one non slopped subframe (left or right)
#define COMPCAPF_HalfCompoundSlop	0x0004		// means compound mode can to cosists from two slopped subframes (left and right)
#define COMPCAPF_HalfCompoundDirect	0x0008		// means compound mode can to consist from one non slopped subframe and one slopped subframe (left or right)
#define COMPCAPF_FullCompound		0x0010		// means compound mode can to consist from three subframes (direct, left, right)
#define COMPCAPF_FullCapability		0x001f
#define COMPCAPF_HalfCompoundDirect2		0x0020		// means compound mode can to consist from four subframes (left, right, left/2, right/2)
#define COMPCAPF_FullCompound2		0x0020		// means compound mode can to consist from five subframes (direct, left, right, left/2, right/2)


// scanning flags
#define COMPOUND_SLOPLEFT			0x0001		// means subframe is slopped left on angle specified
#define COMPOUND_SLOPRIGHT			0x0002		// means subframe is slopped right on angle specified
#define COMPOUND_SLOPNONE			0x0004		// means subframe is not slopped.
#define COMPOUND_HALFSLOPLEFT		0x0008
#define COMPOUND_HALFSLOPRIGHT		0x0010
#define COMPOUND_FULLSCAN (COMPOUND_SLOPLEFT | COMPOUND_SLOPRIGHT | COMPOUND_SLOPNONE)

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */


#endif	//_USGDATA_H_