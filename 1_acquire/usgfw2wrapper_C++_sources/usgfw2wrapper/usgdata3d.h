#ifndef __USGDATA3D_H__
#define __USGDATA3D_H__

/*
file contains definitions of 3D ultrasound data formats
*/

#include "usgdata.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

// structure describes capabilities of 3D scanning
typedef struct _VULTRASOUND_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD FramesCount;		// size of available volume zone in frames
	DWORD BeamsCount;		// size of available frame view area in beams
	DWORD PointsCount;		// size of available beam digitizing zone in points
	DWORD MinInputFrames;	// minimum input frames in volume zone
	DWORD MaxInputFrames;	// maximum input frames in volume zone
	DWORD MinInputBeams;
	DWORD MaxInputBeams;
	DWORD MinInputPoints;
	DWORD MaxInputPoints;
	DWORD MinPointTime;
	DWORD MaxPointTime;
	int CropFramesGranularity;	// granularity on number of input frames number
	int CropBeamsGranularity;
	int CropPointsGranularity;
	int CropPointTimeGranularity;
	int CropAlignFrames;		// granularity of start input frame number
	int CropAlignBeam;
	int CropAlignPoints;
	DWORD MinOutputFrames;		// minimum output frames number
	DWORD MaxOutputFrames;		// maximum output frames number
	DWORD MinOutputBeams;		// 
	DWORD MaxOutputBeams;
	DWORD MinOutputPoints;
	DWORD MaxOutputPoints;
	int StretchTapsFrames;		// indicates how well input frames can be stretched into output frames
	int StretchTapsBeams;
	int StretchTapsPoints;
	int ShrinkTapsFrames;		// indicates how well input frames can be shrinked into output frames
	int ShrinkTapsBeams;
	int ShrinkTapsPoints;
	LONGLONG MinVolumeInterval;
	LONGLONG MaxVolumeInterval;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} VULTRASOUND_STREAM_CONFIG_CAPS, *PVULTRASOUND_STREAM_CONFIG_CAPS;

// structure describes format of 3D ultrasound data 
typedef struct _USGVOLUMEINFOHEADER
{
	DWORD dwSize;		// size of this structure
	DWORD PointsPerBeam;		// number of points per beam
	DWORD BeamsPerFrame;		// 
	DWORD FramesPerVolume;
	DWORD BitCount;
	DWORD Pitch;
	DWORD FrameSize;
	DWORD HeaderSize;		// size of the data header
	DWORD TimePerPoint;
	DWORD TimePerBeam;
} USGVOLUMEINFOHEADER, *PUSGVOLUMEINFOHEADER;


typedef struct _USGBEAM_VECTORS
{
	DWORD dwSize;		// full size of this structure, offset from this structure start to next structure
	DWORD dwFlags;		// 
	DWORD dwElements;	// number of used elements in following array
	USGBEAM_VECTOR UsgBeamVector[1];
} USGBEAM_VECTORS, *PUSGBEAM_VECTORS;

typedef struct _USGFRAME_VECTORS
{
	DWORD dwSize;		// size of this structure, offset to next structure
	DWORD dwFlags;
	DWORD dwElements;	// number of used elements in the following structure
	USGBEAM_VECTOR UsgFrameVectors[1];
} USGFRAME_VECTORS, *PUSGFRAME_VECTORS;

typedef struct _VULTRASOUND_INFO
{
	SIZE szSourceFrames;
	SIZE szSourceBeams;
	SIZE szSourcePoints;
	SIZE szTargetFrames;
	SIZE szTargetBeams;
	SIZE szTargetPoints;
	ULONG PointTime;
	LONGLONG AvgTimePerFrame;
	LONGLONG AvgTimePerVolume;
	USGVOLUMEINFOHEADER UsgVolumeInfoHeader;
	USGBEAM_VECTORS UsgBeamVectors;
	USGFRAME_VECTORS UsgFrameVectors;
} VULTRASOUND_INFO, *PVULTRASOUND_INFO;

typedef struct _VULTRASOUND_INFOHEADER
{
	SIZE szSourceFrames;
	SIZE szSourceBeams;
	SIZE szSourcePoints;
	SIZE szTargetFrames;
	SIZE szTargetBeams;
	SIZE szTargetPoints;
	ULONG PointTime;
	LONGLONG AvgTimePerFrame;
	LONGLONG AvgTimePerVolume;
	USGVOLUMEINFOHEADER UsgVolumeInfoHeader;
} VULTRASOUND_INFOHEADER, *PVULTRASOUND_INFOHEADER;

// this structure is a header some description values that are stored into the
// sample data. ultrasound data follow this header. Most of formats do not use
// data headers
typedef struct _USGDATA_HEADER
{
	DWORD dwDataId;
	DWORD dwDataSize;
} USGDATA_HEADER, *PUSGDATAHEADER;

typedef struct _VULTRASOUND_DATAHEADER
{
	struct _USGDATA_HEADER Hdr;                 // function code indicates get or set.
	DWORD dwFrameIndex;							// index of the frame in the 3D volume
} VULTRASOUND_DATAHEADER, *PVULTRASOUND_DATAHEADER;

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */


#endif	//__USGDATA3D_H__