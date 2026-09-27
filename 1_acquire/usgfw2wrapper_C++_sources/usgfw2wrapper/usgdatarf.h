#ifndef _USGDATARF_H_
#define _USGDATARF_H_

#include "usgdata.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */
#include <pshpack1.h>

typedef struct tagRFUSFILEHEADER {
	DWORD rfType;
	DWORD rfSize;
	DWORD rfVersion;
	ULONG64 rfFileSize;
	DWORD rfFramesNumber;
	DWORD rfReserved;
	ULONG64 rfOffbits;
} RFUSFILEHEADER, *PRFUSFILEHEADER;

typedef struct tagPROBE_DATA {
	DWORD pdSize;
	DWORD pdFlags;
	DWORD pdProbeType;
	DWORD pdElementsNum;
	EXVALUE pdElementPitch;
	EXVALUE pdElementAngle;
} PROBE_DATA, *PPROBE_DATA;

typedef enum tagProbeCode
{	
	PROBECODE_Undefined	= 0,
	PROBECODE_Linear	= 1,
	PROBECODE_Convex	= 2,
	PROBECODE_PhasedArray	= 3,
	PROBECODE_TProbe	= 4,
	PROBECODE_Convex3D	= 5,
	PROBECODE_ProbeDuo	= 6,
} PROBE_CODE;

typedef enum tagPROBE_DATA_FLAGS
{
	PROBE_DATA_PROBETYPE	= 0x01,
	PROBE_DATA_ELEMENTSNUM	= 0x02,
	PROBE_DATA_ELEMENTPITCH	= 0x04,
	PROBE_DATA_ELEMENTANGLE	= 0x08,
} PROBE_DATA_FLAGS;

typedef struct tagRFSCANNING_DETAILS
{
	DWORD rsdSize;				// size of this structure, offset to next data element
	DWORD rsdBeamformerID;		// identifier of the scanner
	DWORD rsdScanningMode;		// identifier of scanning mode.
	DWORD rsdScanningType;		// flags of scanning type (compound, regular, wide view, steering, multifocus ...)
	DWORD rsdRfDataSource;		// identifier of RF source (after Hilbert, SUMM, channel, etc.)
	RECT rsdSourceRect;				// rectangle of scanning, right = first sample; left = last sample; top = first beam; bottom = last beam
	RECT rsdTargetRect;				// where data is placed inside frame described in RFUSGFRAMEINFOHEADER structure
	DWORD rsdSubFramesNum;			// if scanning type is compound this variable points to number of frames that makes full compound frame.
} RFSCANNING_DETAILS, *PRFSCANNING_DETAILS;

// structure describes derection of beam.
// for summed data formats only rpVector member is used.
// for channel data rpVector describes direction of firing pulse
// and rpFirstChannelIndex describes first element of probe that accords to first channel of received data.
typedef struct tagRFUSBEAM_POSITION 
{
	USGBEAM_VECTOR rpVector;
	INT32 rpFirstChannelIndex;
} RFUSBEAM_POSITION, *PRFUSBEAM_POSITION;

 
typedef struct tagRFUSSCAN_POSITION {
	DWORD rspSize;
	DWORD rspCount;
	RFUSBEAM_POSITION rspBeams[1];
} RFUSSCAN_POSITION, *PRFUSSCAN_POSITION;

typedef struct tagFRAME_HEADER {
	DWORD fhSize;
	DWORD fhDataSize;
	DWORD fhFlags;
	DWORD fhFrameMode;				// scanning mode of frame (for multimode files)
	DWORD fhSubFrameIndex;
	REFERENCE_TIME fhStartTime;
	REFERENCE_TIME fhStopTime;
} FRAME_HEADER, *PFRAME_HEADER;

// definitions of structures to describe RF data

typedef struct _RFUSGFRAMEINFOHEADER 
{
	DWORD size;				// size of this structure
	DWORD SamplesPerChannel;
	DWORD ChannelsPerBeam;
	DWORD SubBeamsNumber;	// number of beams that are used to calculate final beam.
	DWORD BeamsPerFrame;
	DWORD SampleBitCount;
	DWORD Pitch;			// offset in bytes between channels.
	DWORD FrameSize;		// frame size
	DWORD TimePerPoint;		// this time describes digitasing time of samples in buffer. accords to rcTarget
	DWORD TimePerBeam;
	DWORD Mask1;
	DWORD Mask2;
	DWORD Mask3;
	DWORD Mask4;
} RFUSGFRAMEINFOHEADER, *PRFUSGFRAMEINFOHEADER;

typedef struct _ULTRASOUNDRF_INFO
{
	RECT rcSource;		// position of received ultrasound data in terms of "virtual beams area * virtual points area"
	ULONG SamplingTime;	// sampling time for samples in rcSource member
	RECT rcTarget;		// description how data is really scanned and write into frame buffer. width can differs from rcSource is points (samples) are scaled, height can differ from rcSource if height is differ. size of ths rectangle accords to frame header members.
	ULONG ScanningFlags;
	DWORD DataCode;		// code of data (a part of 
	LONGLONG AvgTimePerFrame;
	DWORD SubFramesNumber;
	RFUSGFRAMEINFOHEADER UsgFrameInfoHeader;
	RFUSBEAM_POSITION UsgBeamVector[1];
} ULTRASOUNDRF_INFO, *PULTRASOUNDRF_INFO;

typedef struct _ULTRASOUNDRF_INFOHEADER {
	RECT rcSource;
	ULONG SamplingTime;
	RECT rcTarget;
	ULONG ScanningFlags;
	DWORD DataCode;
	LONGLONG AvgTimePerFrame;
	DWORD SubFramesNumber;
	RFUSGFRAMEINFOHEADER UsgFrameInfoHeader;
} ULTRASOUNDRF_INFOHEADER, *PULTRASOUNDRF_INFOHEADER;

typedef struct _ULTRASOUNDRF_STREAM_CONFIG_CAPS
{
	GUID guid;
	DWORD BeamsCount;
	DWORD ChannelsCount;
	DWORD SamplesCount;
	DWORD ScanningCapFlags;
	DWORD MinInputBeams;
	DWORD MaxInputBeams;
	DWORD MinInputChannels;
	DWORD MaxInputChannels;
	DWORD MinInputSamples;
	DWORD MaxInputSamples;
	DWORD MinSampleTime;
	DWORD MaxSampleTime;
	int CropBeamsGranularity;
	int CropChannelsGranularity;
	int CropSamplesGranularity;
	int CropSampleTimeGranularity;
	int CropAlignBeam;
	int CropAlignChannels;
	int CropAlignSamples;
	DWORD MinOutputBeams;
	DWORD MaxOutputBeams;
	DWORD MinOutputChannels;
	DWORD MaxOutputChannels;
	DWORD MinOutputSamples;
	DWORD MaxOutputSamples;
	int StretchTapsBeams;
	int StretchTapsChannels;
	int StretchTapsSamples;
	int ShrinkTapsBeams;
	int ShrinkTapsChannels;
	int ShrinkTapsSamples;
	LONGLONG MinFrameInterval;
	LONGLONG MaxFrameInterval;
	LONG MinBitsPerSecond;
	LONG MaxBitsPerSecond;
} ULTRASOUNDRF_STREAM_CONFIG_CAPS, *PULTRASOUNDRF_STREAM_CONFIG_CAPS;


#include <poppack.h>

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */

#endif		// _USGDATARF_H_