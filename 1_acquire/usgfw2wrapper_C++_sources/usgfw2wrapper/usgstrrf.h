#ifndef _USGSTRRF_H_
#define _USGSTRRF_H_

#include "usgstrtemp.h"
#include "usgdatarf.h"

#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)

// specification of new data format
// {2ED2103E-6A90-40F4-9344-361D90F2F400}
#define STATIC_KSDATAFORMAT_SPECIFIER_ULTRASOUNDINFORF \
0x2ed2103e, 0x6a90, 0x40f4, 0x93, 0x44, 0x36, 0x1d, 0x90, 0xf2, 0xf4, 0x00
	DEFINE_GUIDSTRUCT("2ED2103E-6A90-40F4-9344-361D90F2F400", KSDATAFORMAT_SPECIFIER_ULTRASOUNDINFORF);
#define KSDATAFORMAT_SPECIFIER_ULTRASOUNDINFORF DEFINE_GUIDNAMED(KSDATAFORMAT_SPECIFIER_ULTRASOUNDINFORF)

// identifier for Artus scanner media subtype
// {4DC1A690-55B8-484E-94E1-063AF91431F4}
#define STATIC_KSDATAFORMAT_SUBTYPE_ARTUSDEVICE \
0x4dc1a690, 0x55b8, 0x484e, 0x94, 0xe1, 0x6, 0x3a, 0xf9, 0x14, 0x31, 0xf4
	DEFINE_GUIDSTRUCT("4DC1A690-55B8-484E-94E1-063AF91431F4", KSDATAFORMAT_SUBTYPE_ARTUSDEVICE);
#define KSDATAFORMAT_SUBTYPE_ARTUSDEVICE DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_ARTUSDEVICE)

// identifier for Ultrasound Scan Mode Streaming media subtype
// {55D0683D-2B31-43d9-A7C1-5ED8FCFCCCED}
#define STATIC_KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANMODE \
0x55d0683d, 0x2b31, 0x43d9, 0xa7, 0xc1, 0x5e, 0xd8, 0xfc, 0xfc, 0xcc, 0xed
	DEFINE_GUIDSTRUCT("55D0683D-2B31-43d9-A7C1-5ED8FCFCCCED", KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANMODE);
#define KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANMODE DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANMODE)

// identifier for Ultrasound Source Streaming media subtype
// {903B73AE-AC87-43ec-996A-23170C1E007B}
#define STATIC_KSDATAFORMAT_SUBTYPE_ULTRASOUND_SOURCE \
0x903b73ae, 0xac87, 0x43ec, 0x99, 0x6a, 0x23, 0x17, 0xc, 0x1e, 0x0, 0x7b
	DEFINE_GUIDSTRUCT("903B73AE-AC87-43ec-996A-23170C1E007B", KSDATAFORMAT_SUBTYPE_ULTRASOUND_SOURCE);
#define KSDATAFORMAT_SUBTYPE_ULTRASOUND_SOURCE DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_ULTRASOUND_SOURCE)


// identifier for Ultrasound Scanning Streaming media subtype
// {7774E992-B4F1-46f1-BB38-0D210DBA5EE8}
#define STATIC_KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANNING \
0x7774e992, 0xb4f1, 0x46f1, 0xbb, 0x38, 0xd, 0x21, 0xd, 0xba, 0x5e, 0xe8
	DEFINE_GUIDSTRUCT("7774E992-B4F1-46f1-BB38-0D210DBA5EE8", KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANNING);
#define KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANNING DEFINE_GUIDNAMED(KSDATAFORMAT_SUBTYPE_ULTRASOUND_SCANNING)


// here we will define some new formats and data types.
// typedef USGFRAMEINFO_BMODE KS_BMODE_USGFRAMEINFO, *PKS_BMODE_USGFRAMEINFO;
typedef ULTRASOUNDRF_INFOHEADER KS_ULTRASOUNDRF_INFOHEADER, *PKS_ULTRASOUNDRF_INFOHEADER;
typedef ULTRASOUNDRF_INFO KS_ULTRASOUNDRF_INFO, *PKS_ULTRASOUNDRF_INFO;
typedef ULTRASOUNDRF_STREAM_CONFIG_CAPS KS_ULTRASOUNDRF_STREAM_CONFIG_CAPS, *PKS_ULTRASOUNDRF_STREAM_CONFIG_CAPS;

// диапазон поддерживаемых разрешений для потока ультразвука в режиме B
typedef struct _KS_DATARANGE_ULTRASOUNDRF
{
	KSDATARANGE DataRange;
	BOOL bFixedSizeSample;
	KS_ULTRASOUNDRF_STREAM_CONFIG_CAPS ConfigCaps;		// capabilities
	KS_ULTRASOUNDRF_INFOHEADER UltrasoundInfoHeader;		// default format
} KS_DATARANGE_ULTRASOUNDRF, *PKS_DATARANGE_ULTRASOUNDRF;

typedef struct _KS_DATAFORMAT_ULTRASOUNDRFINFOHEADER
{
	KSDATAFORMAT DataFormat;
	KS_ULTRASOUNDRF_INFOHEADER UltrasoundInfoHeader;
} KS_DATAFORMAT_ULTRASOUNDRFINFOHEADER,*PKS_DATAFORMAT_ULTRASOUNDRFINFOHEADER;

typedef struct _KS_DATAFORMAT_ULTRASOUNDRFINFO
{
	KSDATAFORMAT DataFormat;
	KS_ULTRASOUNDRF_INFO UltrasoundInfo;
} KS_DATAFORMAT_ULTRASOUNDRFINFO,*PKS_DATAFORMAT_ULTRASOUNDRFINFO;


#define USGFW_STREAM_ID_B		0x01	// Id for B-mode streams
#define USGFW_STREAM_ID_B2		0x02	// Id for next B-mode streams
#define USGFW_STREAM_ID_M		0x03	// Id for M-mode streams
#define USGFW_STREAM_ID_CFM		0x04	// Id for CFM streams
#define USGFW_STREAM_ID_PDI		0x05	// Id for PDI streams
#define USGFW_STREAM_ID_DPDI	0x06	// Id for DPDI streams
#define USGFW_STREAM_ID_PW		0x07	// Id for PW (pulse wave) streams
#define USGFW_STREAM_ID_AUDIO	0x08	// Id for PW audio streams
#define USGFW_STREAM_ID_CW		0x09	// Id for CW (continuous wave) streams
#define USGFW_STREAM_ID_CM		0x0a	// Id for CM (color M) streams

#define USGFW_STREAM_ID_RF		0x80	// flag for RF data. it can be combined with all scan mode flags

#include <pshpack1.h>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)


// elementary streaming
// elementary streaming is data streaming of ultrasound data. 
// streaming is a sequence of bytes, bytes are grouped in the packets and packet is a streaming data unit.
// each packet is started with packet header followed with data bytes. packet can to contain different data. packet header is structure that describes what data is in packet.

// stream packet header. stream packets of all types starts with this structure.
// the difference is in dwHeaderPacketID member and bPacketType, that accords to particulal stream specification.

// each packet starts from identifier key (32 bits), data type (usgp_t)(5 bits), number of bytes in length (3bits), data length (number of bytes from previous field).

typedef struct tagUsgsPacketHeader
{
	DWORD dwHeaderPacketID;			// stream header identifier, always USGS. Packet header can be found in the binary stream using this identifier
	BYTE bSizeLegth : 3;			// number of bytes+1 in bSize array. value 0 means 1 byte specifies packet data length, value 7 means 8 bytes specifies packet data size;
	BYTE bPacketType : 5;			// packet type. specifies what data does packet contain.
	BYTE bSize[1];					// size of packet data in bytes
} USGS_PACKET_HEADER, *PUSGS_PACKET_HEADER;

// we have several levels of ultrasound streaming

// specifier MEDIASUBTYPE_UltrasoundScanMode
// elementary scan mode streaming.
// elementary scan mode streaming is binary streaming of single mode ultrasound data. 
// elementary scan mode stream contains from following packets:
// - format packets
// - data frame packets

// identifiers for elementary scan mode stream
#define USMS	MAKEFOURCC('U','S','M','S')		// packet header identifier of scan mode stream
#define USMS_SIZE			4		// length in bytes of packet header identifier.

// type of data in elementary Ultrasound Scan Mode Packet binary packet
enum usmp_t {
	USMP_USGSTREAMBOGUS = 0,		// a set of data must be skipped. bytes inside this packet are not a part of stream
	USMP_USGSTREAMSAMPLE = 1,		// packet contain data of ultrasound frame(s). See USGS_SAMPLE_PACKET structure 
	USMP_USGSTREAMFORMAT = 2,		// packet contain media type of the one of streams of elementary stream. See USGS_FORMAT_PACKET structure 
	USMP_USGPROBEDATA = 3,		// packet contain information structure about a probe USGS_PROBEINFO_PACKET
	USMP_USGLOSTDATA = 4,		// packet contain information structure about lost data.  structure contain number of bytes lost.
};


// data types of ultrasound scan mode streams
typedef struct tagUsmsSamplePacket
{
	// DWORD dwStreamID;				// stream identifier. It is an index from streams map. To know stream mode use streams map.
	DWORD dwSampleSize;				// size of data bytes in the sample
	DWORD dwSampleFlags;			// sample flags. describes what structure members are valid and now this sample relates to other stream samples.
	DWORD dwTypeSpecificFlags;		// sample flags, specific to particular scanning mode. for not 0
	REFERENCE_TIME tStart;			// start time of the sample. this time is specifier relative to scan time
	REFERENCE_TIME tStop;			// stop time of the sample. this time is specified relative to scan time
	REFERENCE_TIME tScanStart;		// scanning start time. absolute time when scanning was started
} USMS_SAMPLE_PACKET, *PUSMS_SAMPLE_PACKET;

enum tagUSGS_SAMPLE_PACKET_FLAGS
{
	USGS_SAMPLE_PACKET_SPLICEPOINT = 0x01,		// this frame can be displayed without reference to other frames
	USGS_SAMPLE_PACKET_PREROLL = 0x02,		// sample is preroll sample. it does not displayed, but it is used to create other sample(s)
	USGS_SAMPLE_PACKET_DATADISCONTINUITY = 0x04,		// sample has new data not related to previous samples. 
	USGS_SAMPLE_PACKET_STARTTIMEVALID = 0x08,
	USGS_SAMPLE_PACKET_STOPTIMEVALID = 0x10,
	USGS_SAMPLE_PACKET_SCANTIMEVALID = 0x40,
	USGS_SAMPLE_PACKET_ENDOFSTREAM = 0x80,		// it is last sample of this stream. used for playbacks
};

typedef struct tagUsmsFormatPacket
{
	GUID majorType;					// 
	GUID subType;					//
	GUID formatType;				// GUID that specifies the structure used for the format block
	DWORD dwSampleSize;				// sample max size. use this value to allocate buffers to receive ultrasound data stream
	DWORD dwFormatSize;				// size of format
} USMS_FORMAT_PACKET, *PUSMS_FORMAT_PACKET;

typedef struct tag_UsgsProbeInfoPacket
{
	DWORD dwFieldsFlags;
	DWORD dwProbeType;
	DWORD dwElementsNum;
	EXVALUE evElementPitch;
	EXVALUE evElementAngle;
} USGS_PROBEINFO_PACKET, *PUSGS_PROBEINFO_PACKET;

enum tagUSGS_PROBE_INFO_FLAGS
{
	USGS_PROBE_INFO_PROBETYPE = 0x01,
	USGS_PROBE_INFO_ELEMENTSNUM = 0x02,
	USGS_PROBE_INFO_ELEMENTPITCH = 0x04,
	USGS_PROBE_INFO_ELEMENTANGLE = 0x08,
};

typedef struct tagUsssFormatPacket
{
	DWORD dwStreamID;				// stream identifier. It is an index from streams map. To know stream mode use streams map.
	USMS_FORMAT_PACKET usmsFormat;	//
} USSS_FORMAT_PACKET, *PUSSS_FORMAT_PACKET;


typedef struct tagUsssMapEntry
{
	DWORD dwStreamID;						// stream identifier specified to the stream by elementary stream source muxer 
	DWORD dwStreamMode;						// scanning mode of ultrasound data stream (USGFW_STREAM_ID_B, USGFW_STREAM_ID_M, USGFW_STREAM_ID_CFM...)
} USSS_MAP_ENTRY, *PUSSS_MAP_ENTRY;

typedef struct tagUsssMapPacket
{
	DWORD dwStreamsNum;					// number of entries in Stream array
	USSS_MAP_ENTRY Streams[1];			// array of stream identifiers.
} USSS_MAP_PACKET, *PUSSS_MAP_PACKET;

typedef struct tagUsssDataPacket
{
	DWORD dwStreamID;
	BYTE bData[1];
} USSS_DATA_PACKET, *PUSSS_DATA_PACKET;

// elementary ultrasound scanning (ultrasound source) streaming
// scanning can to contain of several elementary scan mode streams. Examples are: B+M, B+CFM, B+PW. single modes (B, M, PW) can be packet into ultrasound scanning stream (in this case stream contain only one ultrasound mode stream)
// elementary ultrasound scanning stream contains from following packets:
// - streams description packet. packet contain information of ultrasound scanning mode stream.
// - data packet. packet contain piece of ultrasound scanning mode stream.

// specifier MEDIASUBTYPE_UltrasoundScanning

// identifiers for elementary scan mode stream
#define USSS	MAKEFOURCC('U','S','S','S')		// packet header identifier of scanning stream
#define USSS_SIZE			4		// length in bytes of packet header identifier.

// type of data in elementary Ultrasound Scan Mode Packet binary packet
enum ussp_t {
	USSP_USGSTREAMBOGUS = 0,
	USSP_USGSTREAMDATA = 1,		// packet contain data of ultrasound scan mode stream. packet data starts from DWORD value that describes contain scan mode identifier
	USSP_USGSTREAMFORMAT = 2,		// packet contain media type of the one of streams of elementary stream. See USSS_FORMAT_PACKET structure 
	USSP_USGSTREAMMAP = 3,		// packet contain list of scan mode streams that present in this scanning stream. See USGS_MAP_PACKET structure 
	USSP_USGPROBEDATA = 4,		// packet contain information structure about a probe USGS_PROBEINFO_PACKET
	USSP_USGLOSTDATA = 5,		// packet contain information structure about lost data.  structure contain number of bytes lost.
};

// elementary ultrasound streaming.
// elementary ultrasound scanning stream contain one or more ultrasound scanning streaming.
// elementary ultrasound stream contains from following packets:
// - streams description packet. packet contains information about source streams
// - data packet. packet contain piece of ultrasound source data.

// specifier MEDIASUBTYPE_Ultrasound

// identifiers for elementary scan mode stream
#define USGS	MAKEFOURCC('U','S','G','S')		// packet header identifier of scanning stream
#define USGS_SIZE			4		// length in bytes of packet header identifier.

// type of data in elementary Ultrasound Scan Mode Packet binary packet
enum usgp_t {
	USGP_USGSTREAMBOGUS = 0,
	USGP_USGSTREAMDATA = 1,		// packet contain data of ultrasound scan mode stream. packet data starts from DWORD value that describes contain scanning identifier
	USGP_USGSSOURCESMAP = 2,		// packet contain list of scan mode streams that present in this scanning stream. See USGS_MAP_PACKET structure 
	USGP_USGLOSTDATA = 3,		// packet contain information structure about lost data.  structure contain number of bytes lost.
};

// several streams can be streamed in elementary stream. each stream is specified by Stream ID.
// streams in elementary stream can be captured in different scanning modes.
// All stream in elementary stream has been captured from the same source (scanner) and using same time stamp engine.
// So streams from elementary ultrasound data stream can be mixed to display complex scanning mode. For example when elementary stream
// contains from B and M mode we can to output them in B+M mode output (or B and Color Doopler mode results duplex CFM mode).
// Elementary data stream assumes that data is transfered as a pair of elementary stream header and data block. 
// size and nature of data block is specified in the header. After data block transfer will be finish new data block with header trasfer will be start.


#include <poppack.h>

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif	// _USGSTRRF_H_