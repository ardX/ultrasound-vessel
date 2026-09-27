#ifndef _USGDATAPW_H_
#define _USGDATAPW_H_

#include "usgdata.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

typedef struct _PWULTRASOUND_INFOHEADER {
	LONG FirstPoint;	// first sample volume point
	LONG PointsNumber;	// number of sample volume points
	DWORD BeamNumber;	// PW-line
	LONG PointTime;		// sample volume point's digitizing time
	RECT rcTarget;
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} PWULTRASOUND_INFOHEADER, *PPWULTRASOUND_INFOHEADER;

typedef struct _PWULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD PointsCount;			// scanning area size
	DWORD MinInputPoints;		// volume points
	DWORD MaxInputPoints;		//
	DWORD MinBeamNumber;		// PW-line
	DWORD MaxBeamNumber;		//
	DWORD MinPointTime;			// volume point digitizing time
	DWORD MaxPointTime;			// 
	DWORD MinSampleTime;		// PRF
	DWORD MaxSampleTime;		//
	int CropGranualityBeamNo;	// beam number change granuality
	int CropGranualityPoints;	// volume points change granuality
	int CropAlignPoints;		// volume points align granuality
	int CropGranualityPointTime;	// volume points digitizing granuality
	int CropGranualityBeamTime;		// PRF change granuality
	SIZE MinOutputSize;			// output frame size
	SIZE MaxOutputSize;			//
	int OutputGranualityX;		// output points change granuality
	int OutputGranualityY;
	int StretchTapsX;
	int StretchTapsY;
	int ShrinkTapsX;
	int ShrinkTapsY;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} PWULTRASOUND_STREAM_CONFIG_CAPS, *PPWULTRASOUND_STREAM_CONFIG_CAPS;

typedef struct _CWULTRASOUND_INFOHEADER {
	LONG DepthPoint;	// point to focus scanning. conversion in [mm] - using PointTime
	LONG PointsNumber;	// number of input points
	DWORD BeamNumber;	// PW-line
	LONG PointTime;		// sample volume point's digitizing time
	RECT rcTarget;		// conversion into output points and number of beams in the frame
	LONGLONG AvgTimePerFrame;
	USGFRAMEINFOHEADER UsgFrameInfoHeader;
} CWULTRASOUND_INFOHEADER, *PCWULTRASOUND_INFOHEADER;

typedef struct _CWULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD DepthPoints;			// scanning area size
	DWORD MinInputPoints;		// minimum points to digitize each PRF cycle
	DWORD MaxInputPoints;		// maximum points to digitize each PRF cycle
	DWORD MinBeamNumber;		// PW-line
	DWORD MaxBeamNumber;		//
	DWORD MinPointTime;			// point digitizing time
	DWORD MaxPointTime;			// 
	DWORD MinSampleTime;		// PRF
	DWORD MaxSampleTime;		//
	int CropGranualityBeamNo;	// beam number change granuality
	int CropGranualityPoints;	// volume points change granuality
	int CropAlignPoints;		// volume points align granuality
	int CropGranualityPointTime;	// volume points digitizing granuality
	int CropGranualityBeamTime;		// PRF change granuality
	SIZE MinOutputSize;			// output frame size
	SIZE MaxOutputSize;			//
	int OutputGranualityX;		// output points change granuality
	int OutputGranualityY;
	int StretchTapsX;
	int StretchTapsY;
	int ShrinkTapsX;
	int ShrinkTapsY;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} CWULTRASOUND_STREAM_CONFIG_CAPS, *PCWULTRASOUND_STREAM_CONFIG_CAPS;

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */


#endif	//_USGDATA_H_