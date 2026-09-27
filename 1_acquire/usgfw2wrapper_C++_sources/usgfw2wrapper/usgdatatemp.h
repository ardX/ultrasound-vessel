#ifndef __USGDATATEMP_H__
#define __USGDATATEMP_H__

#include "usgdata.h"
// definitions for virtual convex data formats.
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

// doppler format structures

// Color M

typedef struct _CMULTRASOUND_INFOHEADER {
	LONG FirstPoint;
	LONG PointsNumber;
	LONG PointTime;				// point time for 'FirstPoint' and 'PointsNumber' structure members.
	DWORD BeamNumber;
	RECT rcTarget;
	ULONG BaseLine;
	DWORD dwPRI;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} CMULTRASOUND_INFOHEADER, *PCMULTRASOUND_INFOHEADER;

typedef struct _USGFRAMEINFO_CMMODE {
	USGFRAMEINFOHEADER InfoHeader;
	USGBEAM_VECTOR UsgBeamVector;
	DWORD FrameHeaderSize;
} USGFRAMEINFO_CMMODE, *PUSGFRAMEINFO_CMMODE;

typedef struct _CMULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD PointsCount;
	DWORD PointTime;			// point time for 'PointsCount', 'MinInputPoints', 'MaxInputPoints' structure members. corresponds to CMULTRASOUND_INFOHEADER::PointTime variable.
	DWORD MinInputPoints;
	DWORD MaxInputPoints;
	DWORD MinBeamNumber;
	DWORD MaxBeamNumber;
	DWORD MinBeamTime;
	DWORD MaxBeamTime;
	DWORD MinPointTime;
	DWORD MaxPointTime;
	DWORD MinBaseLine;
	DWORD MaxBaseLine;
	DWORD MinPRI;
	DWORD MaxPRI;
	int CropGranualityBeamNo;			// интервал изменения номера луча от MinBeamNumber до MaxBeamNumber (BeamNumber)
	int CropGranualityPoints;			// интервал изменения количества точек от MinInputPoints до MaxInputPoints. (PointsNumber, PointsPerBeam)
	int CropGranualityBeamTime;			// интервал изменения времени луча (sweep speed) от MinBeamBite до MaxBeamTime (TimePerBeam)
	int CropGranualityPointTime;		// интервал изменения частоты дистретизации от MinPointTime до MaxPointTime (TimePerPoint)
	int CropGranualityBaseLine;			// интервал изменения положения нуля от MinBaseLine до MaxBaseLine (BaseLine)
	int CropGranualityPRI;				// интервал изменения PRI от MinPRI до MaxPRI (dwPRI)
	int CropGranualityFirstPoint;		// интервал изменения первой точки объема (FirstPoint)
	int CropAlignFirstPoint;			// минимальное значение первой точки объема (FirstPoint)
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
} CMULTRASOUND_STREAM_CONFIG_CAPS, *PCMULTRASOUND_STREAM_CONFIG_CAPS;

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */


#endif	// __USGDATATEMP_H__