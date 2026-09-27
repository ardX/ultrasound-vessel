
#ifndef _USGFW_H_
#define _USGFW_H_

#ifndef __strmif_h__
#error strmif.h must be included first
#endif

#include "usgdatapw.h"
#include "usgdatatemp.h"
#include "usgdata3d.h"
//#include "strmif.h"


#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */



//////////////////////////////////////////////////////////////////////////
// ultrasound data media type
//////////////////////////////////////////////////////////////////////////

// {66677375-0000-0010-8000-00aa00389b71}
DEFINE_GUID(MEDIATYPE_Ultrasound, 
0x66677375, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

//////////////////////////////////////////////////////////////////////////
// ultrasound data subtypes
//////////////////////////////////////////////////////////////////////////

// {7CB05E8F-0CCE-4264-B830-02BF594A16BF}
DEFINE_GUID(MEDIASUBTYPE_CFM, 
0x7cb05e8f, 0xcce, 0x4264, 0xb8, 0x30, 0x2, 0xbf, 0x59, 0x4a, 0x16, 0xbf);

// {D3867668-F487-401b-AFB7-640343BAEE43}
DEFINE_GUID(MEDIASUBTYPE_PDI, 
0xd3867668, 0xf487, 0x401b, 0xaf, 0xb7, 0x64, 0x3, 0x43, 0xba, 0xee, 0x43);

// {E1FDE3F8-FDB8-4ca1-8904-BC20BAFA0991}
DEFINE_GUID(MEDIASUBTYPE_DPDI, 
0xe1fde3f8, 0xfdb8, 0x4ca1, 0x89, 0x4, 0xbc, 0x20, 0xba, 0xfa, 0x9, 0x91);

// {CCB22C49-1ADD-4324-B37C-33E11678AF47}
DEFINE_GUID(MEDIASUBTYPE_PW, 
0xccb22c49, 0x1add, 0x4324, 0xb3, 0x7c, 0x33, 0xe1, 0x16, 0x78, 0xaf, 0x47);

// {0ED47143-2F8D-4812-A917-4E0A64D95E69}
DEFINE_GUID(MEDIASUBTYPE_CW, 
0xed47143, 0x2f8d, 0x4812, 0xa9, 0x17, 0x4e, 0xa, 0x64, 0xd9, 0x5e, 0x69);

// {96BBEAF0-7830-4435-9486-896797AB1769}
DEFINE_GUID(MEDIASUBTYPE_CM, 
0x96bbeaf0, 0x7830, 0x4435, 0x94, 0x86, 0x89, 0x67, 0x97, 0xab, 0x17, 0x69);

// {D7CF2498-07BF-4cc1-9E0F-EA84904DA7A8}
DEFINE_GUID(MEDIASUBTYPE_ELAST, 
0xd7cf2498, 0x7bf, 0x4cc1, 0x9e, 0xf, 0xea, 0x84, 0x90, 0x4d, 0xa7, 0xa8);

//////////////////////////////////////////////////////////////////////////
// data format specifiers
//////////////////////////////////////////////////////////////////////////

// specifier for b-mode ultrasound data format
// {CCACFE6B-4A08-4d45-8C24-0899F4B176DD}
DEFINE_GUID(FORMAT_BUltrasoundInfo, 
0xccacfe6b, 0x4a08, 0x4d45, 0x8c, 0x24, 0x8, 0x99, 0xf4, 0xb1, 0x76, 0xdd);

// specifier for m-mode ultrasound data format
// {E1680100-7016-11d7-95E4-00E07D9563F1}
DEFINE_GUID(FORMAT_MUltrasoundInfo, 
0xe1680100, 0x7016, 0x11d7, 0x95, 0xe4, 0x0, 0xe0, 0x7d, 0x95, 0x63, 0xf1);

// specifier for PW-mode ultrasound format
// {3B8B28E7-01F2-44ad-99B9-4ED38D48798B}
DEFINE_GUID(FORMAT_PwUltrasoundInfo, 
0x3b8b28e7, 0x1f2, 0x44ad, 0x99, 0xb9, 0x4e, 0xd3, 0x8d, 0x48, 0x79, 0x8b);

// {8BF30DF2-CAA7-4ab4-B090-E3E72533745D}
DEFINE_GUID(FORMAT_CwUltrasoundInfo, 
0x8bf30df2, 0xcaa7, 0x4ab4, 0xb0, 0x90, 0xe3, 0xe7, 0x25, 0x33, 0x74, 0x5d);

// specifier for CFM-mode ultrasound data format
// {F36BB8FD-3A87-4db8-BEB3-FC3DF3302C03}
DEFINE_GUID(FORMAT_CfmUltrasoundInfo, 
0xf36bb8fd, 0x3a87, 0x4db8, 0xbe, 0xb3, 0xfc, 0x3d, 0xf3, 0x30, 0x2c, 0x3);

// {207572F4-8F75-4b6d-AF84-8DDBBD550423}
DEFINE_GUID(FORMAT_CmUltrasoundInfo, 
0x207572f4, 0x8f75, 0x4b6d, 0xaf, 0x84, 0x8d, 0xdb, 0xbd, 0x55, 0x4, 0x23);

// {2ED2103E-6A90-40F4-9344-361D90F2F400}
DEFINE_GUID(FORMAT_RfUltrasoundInfo, 
			0x2ed2103e, 0x6a90, 0x40f4, 0x93, 0x44, 0x36, 0x1d, 0x90, 0xf2, 0xf4, 0x00);

// specifier for 3D ultrasound data format
// {47CFCAEF-2EC6-4d28-8226-86A713D333A4}
DEFINE_GUID(FORMAT_3DUltrasoundInfo, 
0x47cfcaef, 0x2ec6, 0x4d28, 0x82, 0x26, 0x86, 0xa7, 0x13, 0xd3, 0x33, 0xa4);

// definition of new format GUID
// {958BE04D-A92C-4fe7-8F7B-F93715066323}
DEFINE_GUID(FORMAT_UltrasoundInfo2D,
			0x958be04d, 0xa92c, 0x4fe7, 0x8f, 0x7b, 0xf9, 0x37, 0x15, 0x6, 0x63, 0x23);

// definition for 2D ultrasound scanning media subtype
// {4351D676-A60D-40e5-A4E2-1DECE0B17706}
DEFINE_GUID(MEDIASUBTYPE_VirtualConvex2D, 
			0x4351d676, 0xa60d, 0x40e5, 0xa4, 0xe2, 0x1d, 0xec, 0xe0, 0xb1, 0x77, 0x6);

// {1A81084A-F70D-4804-88CA-9E65BA4E3CB0}
DEFINE_GUID(MEDIASUBTYPE_Compound2D, 
			0x1a81084a, 0xf70d, 0x4804, 0x88, 0xca, 0x9e, 0x65, 0xba, 0x4e, 0x3c, 0xb0);

// {F1661EDE-2EA2-4850-B0BD-DAF1DDDE0FA0}
DEFINE_GUID(MEDIASUBTYPE_Steering2D, 
			0xf1661ede, 0x2ea2, 0x4850, 0xb0, 0xbd, 0xda, 0xf1, 0xdd, 0xde, 0xf, 0xa0);

// {8C409C86-FBCC-48b7-B4BD-1392E302A7F5}
DEFINE_GUID(MEDIASUBTYPE_DeviceSpecific, 
			0x8c409c86, 0xfbcc, 0x48b7, 0xb4, 0xbd, 0x13, 0x92, 0xe3, 0x2, 0xa7, 0xf5);

// {4DC1A690-55B8-484E-94E1-063AF91431F4}
DEFINE_GUID(MEDIASUBTYPE_ArtusDeviceRF, 
			0x4dc1a690, 0x55b8, 0x484e, 0x94, 0xe1, 0x6, 0x3a, 0xf9, 0x14, 0x31, 0xf4);


// elementary ultrasound scan mode streaming
// {55D0683D-2B31-43d9-A7C1-5ED8FCFCCCED}
DEFINE_GUID(MEDIASUBTYPE_UltrasoundScanMode, 
			0x55d0683d, 0x2b31, 0x43d9, 0xa7, 0xc1, 0x5e, 0xd8, 0xfc, 0xfc, 0xcc, 0xed);

// elementary ultrasound scanning streaming
// {903B73AE-AC87-43ec-996A-23170C1E007B}
DEFINE_GUID(MEDIASUBTYPE_UltrasoundScanning, 
			0x903b73ae, 0xac87, 0x43ec, 0x99, 0x6a, 0x23, 0x17, 0xc, 0x1e, 0x0, 0x7b);

// general elementary ultrasound streaming 
// {7774E992-B4F1-46f1-BB38-0D210DBA5EE8}
DEFINE_GUID(MEDIASUBTYPE_Ultrasound, 
			0x7774e992, 0xb4f1, 0x46f1, 0xbb, 0x38, 0xd, 0x21, 0xd, 0xba, 0x5e, 0xe8);

/////////////////////////////////////////////////////////////////////////////
// definition of the units of measurements base
/////////////////////////////////////////////////////////////////////////////

// specifier for units that are codes
// {1D904B36-BC06-48c4-85A5-4F1BE1431220}
DEFINE_GUID(GUID_Format_Code,
0x1d904b36, 0xbc06, 0x48c4, 0x85, 0xa5, 0x4f, 0x1b, 0xe1, 0x43, 0x12, 0x20);

// specifier for units that are time in seconds
// {F3A52845-E874-4c7e-9C72-31D7A679134F}
DEFINE_GUID(GUID_Format_Second,
0xf3a52845, 0xe874, 0x4c7e, 0x9c, 0x72, 0x31, 0xd7, 0xa6, 0x79, 0x13, 0x4f);

// specifier for units that are distance in meters
// {AEC662F4-9BBD-4f93-8FC4-A3A9561AE8EA}
DEFINE_GUID(GUID_Format_Meter,
0xaec662f4, 0x9bbd, 0x4f93, 0x8f, 0xc4, 0xa3, 0xa9, 0x56, 0x1a, 0xe8, 0xea);

// specifier for units that are attenuation in bells
// {4DEDEEF9-E61C-4b51-B619-E39F6807C0C3}
DEFINE_GUID(GUID_Format_Bell,
0x4dedeef9, 0xe61c, 0x4b51, 0xb6, 0x19, 0xe3, 0x9f, 0x68, 0x7, 0xc0, 0xc3);

// specifier for units that are frequency in hertz
// {8D0973C5-C98E-43b7-8189-AF2952B49A41}
DEFINE_GUID(GUID_Format_Hertz,
0x8d0973c5, 0xc98e, 0x43b7, 0x81, 0x89, 0xaf, 0x29, 0x52, 0xb4, 0x9a, 0x41);

// specifier for units that are	weight in gramms
// {33E800B1-0165-4b5f-9711-DE597B33171C}
DEFINE_GUID(GUID_Format_Gramm,
0x33e800b1, 0x165, 0x4b5f, 0x97, 0x11, 0xde, 0x59, 0x7b, 0x33, 0x17, 0x1c);

// specifier for units that are percents
// {6B69E131-767E-4ccc-A406-2B0041284ACE}
DEFINE_GUID(GUID_Format_Percent,
0x6b69e131, 0x767e, 0x4ccc, 0xa4, 0x6, 0x2b, 0x0, 0x41, 0x28, 0x4a, 0xce);

// specifier for units that are voltage
// {8FED29EC-4D14-4fb4-ACEC-55CEC85FB7FA}
DEFINE_GUID(GUID_Format_Volt,
0x8fed29ec, 0x4d14, 0x4fb4, 0xac, 0xec, 0x55, 0xce, 0xc8, 0x5f, 0xb7, 0xfa);

// {7AA73645-C54C-4e77-B916-9BA7F8FADDB8}
DEFINE_GUID(IID_IUsgMFormatControl, 
0x7aa73645, 0xc54c, 0x4e77, 0xb9, 0x16, 0x9b, 0xa7, 0xf8, 0xfa, 0xdd, 0xb8);
DECLARE_INTERFACE_(IUsgMFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		MULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};

// {59360BE6-6E2F-44b6-A3FB-5F64F9BAE9BA}
DEFINE_GUID(IID_IUsgBFormatControl, 
0x59360be6, 0x6e2f, 0x44b6, 0xa3, 0xfb, 0x5f, 0x64, 0xf9, 0xba, 0xe9, 0xba);
DECLARE_INTERFACE_(IUsgBFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		BULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};

// {59B43072-DC2D-44e6-97D2-6D43EA2BC985}
DEFINE_GUID(IID_IUsg2dFormatControl, 
0x59b43072, 0xdc2d, 0x44e6, 0x97, 0xd2, 0x6d, 0x43, 0xea, 0x2b, 0xc9, 0x85);
DECLARE_INTERFACE_(IUsg2dFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		int nIndex,
		AM_MEDIA_TYPE **ppMediaType,
		ULTRASOUND2D_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};


// {EB02689B-B1F7-4c37-B4E6-B01BE32428F9}
DEFINE_GUID(IID_IUsgPwFormatControl, 
0xeb02689b, 0xb1f7, 0x4c37, 0xb4, 0xe6, 0xb0, 0x1b, 0xe3, 0x24, 0x28, 0xf9);
DECLARE_INTERFACE_(IUsgPwFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		PWULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};

// {DA4E3F0D-8AE8-4152-985D-916A327DEC39}
DEFINE_GUID(IID_IUsgCwFormatControl, 
0xda4e3f0d, 0x8ae8, 0x4152, 0x98, 0x5d, 0x91, 0x6a, 0x32, 0x7d, 0xec, 0x39);
DECLARE_INTERFACE_(IUsgCwFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		CWULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};


// {8760B564-23C9-4956-AEAE-564647B54CCA}
DEFINE_GUID(IID_IUsgCfmFormatControl, 
0x8760b564, 0x23c9, 0x4956, 0xae, 0xae, 0x56, 0x46, 0x47, 0xb5, 0x4c, 0xca);
DECLARE_INTERFACE_(IUsgCfmFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		CFMULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};

// {85800625-FBEE-4ffd-88E5-5669C343BF61}
DEFINE_GUID(IID_IUsgCmFormatControl, 
0x85800625, 0xfbee, 0x4ffd, 0x88, 0xe5, 0x56, 0x69, 0xc3, 0x43, 0xbf, 0x61);
DECLARE_INTERFACE_(IUsgCmFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		CMULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};


// {A2DFAE2E-940D-4097-B873-AF17BA12C14D}
DEFINE_GUID(IID_IUsg3dFormatControl, 
0xa2dfae2e, 0x940d, 0x4097, 0xb8, 0x73, 0xaf, 0x17, 0xba, 0x12, 0xc1, 0x4d);
DECLARE_INTERFACE_(IUsg3dFormatControl,IUnknown)
{
	STDMETHOD(GetStreamCaps) (THIS_
		AM_MEDIA_TYPE **ppMediaType,
		VULTRASOUND_STREAM_CONFIG_CAPS *pStreamConfigCaps
		) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD(SetCurrentFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
	STDMETHOD(ReleaseFormat) (THIS_
		AM_MEDIA_TYPE *pMediaFormat
		) PURE;
};

// {58DFD942-AAA0-49e8-A9A2-E6FB69DC2CA8}
DEFINE_GUID(IID_IEnumFormats, 
0x58dfd942, 0xaaa0, 0x49e8, 0xa9, 0xa2, 0xe6, 0xfb, 0x69, 0xdc, 0x2c, 0xa8);
DECLARE_INTERFACE_(IEnumFormats,IUnknown)
{
	STDMETHOD(Next) (THIS_
		ULONG celt,
		PDATAFORMAT_BOUNDS pFormats,
		ULONG *pceltFetched
		) PURE;
	STDMETHOD(Skip) (THIS_
		ULONG celt
		) PURE;
	
	
	
	
	STDMETHOD(Reset) (THIS_) PURE;





	STDMETHOD(Clone) (THIS_
		IEnumFormats **pEnum
		) PURE;
};


// {BD93C197-CFA1-4d94-A48C-9B70B3ABFBE8}
DEFINE_GUID(IID_IPropSerialize, 
0xbd93c197, 0xcfa1, 0x4d94, 0xa4, 0x8c, 0x9b, 0x70, 0xb3, 0xab, 0xfb, 0xe8);
DECLARE_INTERFACE_(IPropSerialize,IUnknown)
{
	// serialization
	STDMETHOD(GetSerializeSize) (THIS_
		ULONG *size										// serialize size
		) PURE;
	STDMETHOD(Serialize) (THIS_
		PVOID data,										// data buffer pointer
		ULONG size,										// size of data buffer
		ULONG *cbProcessed							// number of processed bytes
		) PURE;
	STDMETHOD(Unserialize) (THIS_
		PVOID data,										// data buffer pointer
		ULONG size,										// size of data buffer
		ULONG *cbProcessed							// number of processed bytes
		) PURE;
};

// {EFCAF8A7-636E-4860-9EB8-A55F2014C860}
DEFINE_GUID(IID_IPropSerializeRaw, 
0xefcaf8a7, 0x636e, 0x4860, 0x9e, 0xb8, 0xa5, 0x5f, 0x20, 0x14, 0xc8, 0x60);
DECLARE_INTERFACE_(IPropSerializeRaw,IUnknown)
{
	STDMETHOD(SerializeRaw) (THIS_
		) PURE;
	STDMETHOD(UnserializeRaw) (THIS_
		) PURE;
};

// {F8800BD3-E5E9-4671-8A1B-BC02267F8A74}
DEFINE_GUID(IID_IUltrasonicTgc, 
0xf8800bd3, 0xe5e9, 0x4671, 0x8a, 0x1b, 0xbc, 0x2, 0x26, 0x7f, 0x8a, 0x74);
DECLARE_INTERFACE_(IUltrasonicTgc,IUnknown)
{
	// TgcData
	STDMETHOD(SetTgcData) (THIS_
		LONG *pTgcValues,					// массив со значени€ми TGC
		ULONG uStartIndex,
		ULONG cValuesNum,					// к-во элементов (LONG) в массиве pTgcValues
		ULONG *cValuesProcessed			// к-во обработанных байт / к-во необходимых байт в массиве
		) PURE;
	STDMETHOD(GetTgcData) (THIS_
		LONG *pTgcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetTgcDataDefault) (THIS_
		LONG *pTgcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetTgcDataRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// Tgc Keys
	STDMETHOD(SetTgcKeys) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cProcessed
		) PURE;
	STDMETHOD(GetTgcKeys) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetTgcKeysDefault) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetTgcKeysRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// keys
	STDMETHOD(SetKeysNum) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNum) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNumDefault) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNumRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// TGC formats
	STDMETHOD(SetTgcFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetTgcFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetTgcFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumTgcFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// key formats
	STDMETHOD(SetKeyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetKeyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetKeyFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumKeyFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertTgcToFormat) (THIS_
		LONG *pTgcValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertTgcFromFormat) (THIS_
		LONG *pTgcValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;

	STDMETHOD(ConvertKeysToFormat) (THIS_
		LONG *pKeyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertKeysFromFormat) (THIS_
		LONG *pKeyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
};

// {E3689E40-EC5A-462a-BBFA-211AF21B5DE8}
DEFINE_GUID(IID_IUltrasonicGain, 
0xe3689e40, 0xec5a, 0x462a, 0xbb, 0xfa, 0x21, 0x1a, 0xf2, 0x1b, 0x5d, 0xe8);

DECLARE_INTERFACE_(IUltrasonicGain,IUnknown)
{
	// gain
	STDMETHOD(GetGain) (THIS_
		LONG *gain					// get current gain
		) PURE;
	STDMETHOD(SetGain) (THIS_
		LONG *gain					// set current gain
		) PURE;
	STDMETHOD(GetGainDefault) (THIS_
		LONG *gain					// get default gain
		) PURE;
	STDMETHOD(GetGainRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

	// gain formats
	STDMETHOD(SetGainFormat) (THIS_
		PDATAFORMAT pDataFormat	// set format
		) PURE;
	STDMETHOD(GetGainFormat) (THIS_
		PDATAFORMAT pDataFormat	// get format
		) PURE;
	STDMETHOD(GetGainFormatDefault) (THIS_
		PDATAFORMAT pDataFormat	// default format
		) PURE;
	STDMETHOD(EnumGainFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	// convert from current to specified format
	STDMETHOD(ConvertGainToFormat) (THIS_
		LONG *pGainValues,
		ULONG cValuesNum,
		PDATAFORMAT pGainFormat,
		ULONG *cbProcessed
		) PURE;
	// convert from specified to current format
	STDMETHOD(ConvertGainFromFormat) (THIS_
		LONG *pGainValues,
		ULONG cValuesNum,
		PDATAFORMAT pGainFormat,
		ULONG *cbProcessed
		) PURE;
};

// {31D5D3E0-961C-486a-999D-FAB225E482AB}
DEFINE_GUID(IID_IUltrasonicPower, 
0x31d5d3e0, 0x961c, 0x486a, 0x99, 0x9d, 0xfa, 0xb2, 0x25, 0xe4, 0x82, 0xab);

DECLARE_INTERFACE_(IUltrasonicPower,IUnknown)
{
	// power
	STDMETHOD(GetPower) (THIS_
		LONG *power					// The current power
		) PURE;
	STDMETHOD(SetPower) (THIS_
		LONG *power					// The current power
		) PURE;
	STDMETHOD(GetPowerDefault) (THIS_
		ULONG *power
		) PURE;
	STDMETHOD(GetPowerRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// power formats
	STDMETHOD(SetPowerFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetPowerFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetPowerFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumPowerFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertPowerToFormat) (THIS_
		LONG *pPowerValues,
		ULONG cValuesNum,
		PDATAFORMAT pPowerFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertPowerFromFormat) (THIS_
		LONG *pPowerValues,
		ULONG cValuesNum,
		PDATAFORMAT pPowerFormat,
		ULONG *cbProcessed
		) PURE;
};

// {D8493CF6-4418-4c4f-9587-D237241EA712}
DEFINE_GUID(IID_IUltrasonicDynamicRange, 
0xd8493cf6, 0x4418, 0x4c4f, 0x95, 0x87, 0xd2, 0x37, 0x24, 0x1e, 0xa7, 0x12);

DECLARE_INTERFACE_(IUltrasonicDynamicRange,IUnknown)
{
	// DynamicRange
	STDMETHOD(GetDRange) (THIS_
		LONG *drange
		) PURE;
	STDMETHOD(SetDRange) (THIS_
		LONG *drange
		) PURE;
	STDMETHOD(GetDRangeDefault) (THIS_
		ULONG *drange
		) PURE;
	STDMETHOD(GetDRangeRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// dynamic range formats
	STDMETHOD(SetDRangeFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetDRangeFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetDRangeFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumDRangeFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertDRangeToFormat) (THIS_
		LONG *pDRangeValues,
		ULONG cValuesNum,
		PDATAFORMAT pDRangeFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertDRangeFromFormat) (THIS_
		LONG *pDRangeValues,
		ULONG cValuesNum,
		PDATAFORMAT pDRangeFormat,
		ULONG *cbProcessed
		) PURE;
};

// {1B6ED9DD-9F5B-465c-9FB7-9602E1C9F3D0}
DEFINE_GUID(IID_IUltrasonicFocus, 
0x1b6ed9dd, 0x9f5b, 0x465c, 0x9f, 0xb7, 0x96, 0x2, 0xe1, 0xc9, 0xf3, 0xd0);
DECLARE_INTERFACE_(IUltrasonicFocus,IUnknown)
{
	// focus
	STDMETHOD(GetFocus) (THIS_
		ULONG focusset,
		FOCUS_DATA *focus,
		ULONG cbData,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(SetFocus) (THIS_
		ULONG focusset,
		FOCUS_DATA *focus,
		ULONG cbData,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetFocusDefault) (THIS_
		ULONG focusset,
		FOCUS_DATA *focus,
		ULONG cbData,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetFocusesNum)(THIS_		// получает к-во фокусов в наборе
		ULONG focusset,
		ULONG* focusesnum
		)PURE;

	// FocusSet
	STDMETHOD(GetFocusSet) (THIS_
		ULONG *focusset
		) PURE;
	STDMETHOD(SetFocusSet) (THIS_
		ULONG *focusset
		) PURE;
	STDMETHOD(GetFocusSetDefault) (THIS_
		ULONG *focusset
		) PURE;
	STDMETHOD(GetFocusSetNum) (THIS_
		ULONG *focusset
		) PURE;

	// focus description
	STDMETHOD(GetFocusDesc) (THIS_
		ULONG focusset,
		FOCUSES_DESC *focusesdesc,
		ULONG cbData,
		ULONG *cbProcessed
		) PURE;

	// focus description format
	STDMETHOD(SetFocusDescFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFocusDescFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFocusDescFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumFocusDescFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

};

// {C0388DE0-0988-4011-9A11-83A6D9360632}
DEFINE_GUID(IID_IUltrasonicFocus2, 
0xc0388de0, 0x988, 0x4011, 0x9a, 0x11, 0x83, 0xa6, 0xd9, 0x36, 0x6, 0x32);
DECLARE_INTERFACE_(IUltrasonicFocus2,IUnknown)
{
	// focus
	STDMETHOD(GetFocus) (THIS_
		ULONG focusset,
		ULONG *firstFocusIndex,
		ULONG *enabledFocusesNum
		) PURE;
	STDMETHOD(SetFocus) (THIS_
		ULONG focusset,
		ULONG firstFocusIndex,
		ULONG enabledFocusesNum
		) PURE;
	STDMETHOD(GetFocusDefault) (THIS_
		ULONG focusset,
		ULONG *firstFocusIndex,
		ULONG *enabledFocusesNum
		) PURE;
	STDMETHOD(GetFocusesNum) (THIS_
		ULONG focusset,
		ULONG *focusesnum
		) PURE;

	// focus sets
	STDMETHOD(GetFocusSet)(THIS_
		ULONG *focusset
		) PURE;
	STDMETHOD(SetFocusSet) (THIS_
		ULONG focusset
		) PURE;
	STDMETHOD(GetFocusSetDefault)(THIS_
		ULONG *focusset
		) PURE;
	STDMETHOD(GetFocusSetNum) (THIS_
		ULONG *focussetnum
		) PURE;

	// focus set mode
	STDMETHOD(GetFocusSetMode) (THIS_
		ULONG focusset,
		ULONG *transmitMode,
		ULONG *receiveMode
		) PURE;

	// focus description
	STDMETHOD(GetFocusDesc) (THIS_
		ULONG focusset,
		FOCUSES_DESC *focusesdesc,
		ULONG cbData,
		ULONG *cbProcessed
		) PURE;

	// focus description format
	STDMETHOD(SetFocusDescFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFocusDescFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFocusDescFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumFocusDescFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

};


// {4091A317-4E2E-46f6-AB8C-E8CCE78EDA42}
DEFINE_GUID(IID_IUltrasonicAgc, 
0x4091a317, 0x4e2e, 0x46f6, 0xab, 0x8c, 0xe8, 0xcc, 0xe7, 0x8e, 0xda, 0x42);
DECLARE_INTERFACE_(IUltrasonicAgc,IUnknown)
{
	// AgcData
	STDMETHOD(SetAgcData) (THIS_
		LONG *pAgcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetAgcData) (THIS_
		LONG *pAgcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetAgcDataDefault) (THIS_
		LONG *pAgcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetAgcDataRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// Agc Keys
	STDMETHOD(SetAgcKeys) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cProcessed
		) PURE;
	STDMETHOD(GetAgcKeys) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetAgcKeysDefault) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetAgcKeysRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// keys
	STDMETHOD(SetKeysNum) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNum) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNumDefault) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNumRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// AGC formats
	STDMETHOD(SetAgcFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetAgcFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetAgcFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumAgcFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// key formats
	STDMETHOD(SetKeyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetKeyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetKeyFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumKeyFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertAgcToFormat) (THIS_
		LONG *pAgcValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertAgcFromFormat) (THIS_
		LONG *pAgcValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;

	STDMETHOD(ConvertKeysToFormat) (THIS_
		LONG *pKeyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertKeysFromFormat) (THIS_
		LONG *pKeyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
};

// {124E48D0-2FA5-4cea-A530-A5B6F42863FA}
DEFINE_GUID(IID_IUltrasonicTfc, 
0x124e48d0, 0x2fa5, 0x4cea, 0xa5, 0x30, 0xa5, 0xb6, 0xf4, 0x28, 0x63, 0xfa);
DECLARE_INTERFACE_(IUltrasonicTfc,IUnknown)
{
	// TfcData
	STDMETHOD(SetTfcData) (THIS_
		LONG *pTfcValues,					// массив со значени€ми TGC
		ULONG uStartIndex,
		ULONG cValuesNum,					// к-во элементов (LONG) в массиве pTgcValues
		ULONG *cValuesProcessed			// к-во обработанных байт / к-во необходимых байт в массиве
		) PURE;
	STDMETHOD(GetTfcData) (THIS_
		LONG *pTfcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetTfcDataDefault) (THIS_
		LONG *pTfcValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetTfcDataRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// Tfc Keys
	STDMETHOD(SetTfcKeys) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cProcessed
		) PURE;
	STDMETHOD(GetTfcKeys) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(GetTfcKeysDefault) (THIS_
		LONG *pKeyValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG *cValuesProcessed
		) PURE;
	STDMETHOD(GetTfcKeysRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// keys
	STDMETHOD(SetKeysNum) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNum) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNumDefault) (THIS_
		ULONG *KeysNum
		) PURE;
	STDMETHOD(GetKeysNumRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// TFC formats
	STDMETHOD(SetTfcFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetTfcFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetTfcFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumTfcFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// key formats
	STDMETHOD(SetKeyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetKeyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetKeyFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumKeyFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertTfcToFormat) (THIS_
		LONG *pTfcValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertTfcFromFormat) (THIS_
		LONG *pTfcValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;

	STDMETHOD(ConvertKeysToFormat) (THIS_
		LONG *pKeyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertKeysFromFormat) (THIS_
		LONG *pKeyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
};

// {6A2FAAA6-02BD-40a3-9B97-8047EE9EED90}
DEFINE_GUID(IID_IUltrasonicPulseDuration, 
0x6a2faaa6, 0x2bd, 0x40a3, 0x9b, 0x97, 0x80, 0x47, 0xee, 0x9e, 0xed, 0x90);

DECLARE_INTERFACE_(IUltrasonicPulseDuration,IUnknown)
{
	// Pulse duration
	STDMETHOD(GetPDuration) (THIS_
		LONG *pduration
		) PURE;
	STDMETHOD(SetPDuration) (THIS_
		LONG *pduration
		) PURE;
	STDMETHOD(GetPDurationDefault) (THIS_
		ULONG *pduration
		) PURE;
	STDMETHOD(GetPDurationRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// pulse duration formats
	STDMETHOD(SetPDurationFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetPDurationFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetPDurationFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumPDurationFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertPDurationToFormat) (THIS_
		LONG *pPDurationValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertPDurationFromFormat) (THIS_
		LONG *pPDurationValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
};

// {39A42DC2-E402-4555-ACF2-A17DBF0F9A96}
DEFINE_GUID(IID_IUltrasonicFocusGain, 
0x39a42dc2, 0xe402, 0x4555, 0xac, 0xf2, 0xa1, 0x7d, 0xbf, 0xf, 0x9a, 0x96);
DECLARE_INTERFACE_(IUltrasonicFocusGain,IUnknown)
{
	STDMETHOD(SetFocusGain) (THIS_
		ULONG FocusSet,
		LONG *pGainValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG* cValueProcessed
		) PURE;
	STDMETHOD(GetFocusGain) (THIS_
		ULONG FocusSet,
		LONG *pGainValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG* cValueProcessed
		) PURE;
	STDMETHOD(GetFocusGainDefault) (THIS_
		ULONG FocusSet,
		LONG *pGainValues,
		ULONG uStartIndex,
		ULONG cValuesNum,
		ULONG* cValueProcessed
		) PURE;
	STDMETHOD(GetFocusGainRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;

	// focus gain formats support
	STDMETHOD(SetFocusGainFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFocusGainFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFocusGainFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumFocusGainFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;
};

// interface definition to operate with probe frequency

// {32721FB3-5CD6-43af-93A9-476083D30374}
DEFINE_GUID(IID_IUltrasonicProbeFrequency,
0x32721fb3, 0x5cd6, 0x43af, 0x93, 0xa9, 0x47, 0x60, 0x83, 0xd3, 0x3, 0x74);

DECLARE_INTERFACE_(IUltrasonicProbeFrequency,IUnknown)
{
	// Probe frequency
	STDMETHOD(GetFrequency) (THIS_
		LONG *pFrequency
		) PURE;
	STDMETHOD(SetFrequency) (THIS_
		LONG *pFrequency
		) PURE;
	STDMETHOD(GetFrequencyDefault) (THIS_
		LONG *pFrequency
		) PURE;
	// interface to get regular range of available probe
	// frequencies, if supported
	STDMETHOD(GetFrequencyRange) (THIS_
		VALUE_RANGE *pRange
		) PURE;
	// methods to get exact values of supported
	// frequencies
	STDMETHOD(GetFrequenciesNumber)(THIS_
		ULONG *pFreqNumber
		) PURE;
	STDMETHOD(GetFrequenciesValues)(THIS_
		LONG nIndexFirst, 
		LONG nEntries, 
		LONG* pValues,
		LONG* cFetched
		) PURE;


	// probe frequency formats
	STDMETHOD(SetFrequencyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFrequencyFormat) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(GetFrequencyFormatDefault) (THIS_
		PDATAFORMAT pDataFormat
		) PURE;
	STDMETHOD(EnumFrequencyFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	STDMETHOD(ConvertFrequencyToFormat) (THIS_
		LONG *pFrequencyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
	STDMETHOD(ConvertFrequencyFromFormat) (THIS_
		LONG *pFrequencyValues,
		ULONG cValuesNum,
		PDATAFORMAT pDataFormat,
		ULONG *cbProcessed
		) PURE;
};


// {AB71286A-5755-4a67-94FB-9023537196AD}
DEFINE_GUID(IID_IUltrasonicProbeFrequency2, 
0xab71286a, 0x5755, 0x4a67, 0x94, 0xfb, 0x90, 0x23, 0x53, 0x71, 0x96, 0xad);

DECLARE_INTERFACE_(IUltrasonicProbeFrequency2,IUnknown)
{
	// Probe frequency
	STDMETHOD(GetFrequency) (THIS_
		FREQUENCY_HARMONIC *pFrequency
		) PURE;
	STDMETHOD(SetFrequency) (THIS_
		FREQUENCY_HARMONIC *pFrequency
		) PURE;
	STDMETHOD(GetFrequencyDefault) (THIS_
		FREQUENCY_HARMONIC *pFrequency
		) PURE;
	// methods to get exact values of supported
	// frequencies
	STDMETHOD(GetFrequenciesNumber)(THIS_
		ULONG *pFreqNumber
		) PURE;
	STDMETHOD(GetFrequenciesValues)(THIS_
		LONG nIndexFirst, 
		LONG nEntries, 
		FREQUENCY_HARMONIC* pValues,
		LONG* cFetched
		) PURE;
};

// {F4024DCE-B237-46aa-BFCB-5453C104A3D0}
DEFINE_GUID(IID_IUltrasonicWallFilter, 
0xf4024dce, 0xb237, 0x46aa, 0xbf, 0xcb, 0x54, 0x53, 0xc1, 0x4, 0xa3, 0xd0);

DECLARE_INTERFACE_(IUltrasonicWallFilter, IUnknown)
{
	// Wall Filter
	STDMETHOD(GetWallFilter) (THIS_
		LONG *value					// get current wall filter
		) PURE;
	STDMETHOD(SetWallFilter) (THIS_
		LONG *value					// set current wall filter
		) PURE;
	STDMETHOD(GetWallFilterDefault) (THIS_
		LONG *value					// get default wall filter
		) PURE;
	STDMETHOD(GetWallFilterRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

	// gain formats
	STDMETHOD(SetWallFilterFormat) (THIS_
		PDATAFORMAT pDataFormat	// set format
		) PURE;
	STDMETHOD(GetWallFilterFormat) (THIS_
		PDATAFORMAT pDataFormat	// get format
		) PURE;
	STDMETHOD(GetWallFilterFormatDefault) (THIS_
		PDATAFORMAT pDataFormat	// default format
		) PURE;
	STDMETHOD(EnumWallFilterFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	// convert from current to specified format
	STDMETHOD(ConvertWallFilterToFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
	// convert from specified to current format
	STDMETHOD(ConvertWallFilterFromFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
};

// {59628EE9-1ADF-47fa-8394-F35B5B34BBD9}
DEFINE_GUID(IID_IUltrasonicPRF, 
0x59628ee9, 0x1adf, 0x47fa, 0x83, 0x94, 0xf3, 0x5b, 0x5b, 0x34, 0xbb, 0xd9);

DECLARE_INTERFACE_(IUltrasonicPRF, IUnknown)
{
	// PRF
	STDMETHOD(GetPRF) (THIS_
		LONG *value					// get current PRF
		) PURE;
	STDMETHOD(SetPRF) (THIS_
		LONG *value					// set current PRF
		) PURE;
	STDMETHOD(GetPRFDefault) (THIS_
		LONG *value					// get default PRF
		) PURE;
	STDMETHOD(GetPRFRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

	// formats
	STDMETHOD(SetPRFFormat) (THIS_
		PDATAFORMAT pDataFormat	// set format
		) PURE;
	STDMETHOD(GetPRFFormat) (THIS_
		PDATAFORMAT pDataFormat	// get format
		) PURE;
	STDMETHOD(GetPRFFormatDefault) (THIS_
		PDATAFORMAT pDataFormat	// default format
		) PURE;
	STDMETHOD(EnumPRFFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	// convert from current to specified format
	STDMETHOD(ConvertPRFToFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
	// convert from specified to current format
	STDMETHOD(ConvertPRFFromFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
};

// {033ED68B-DC75-4782-B3C1-6ACF560C74B2}
DEFINE_GUID(IID_IUltrasonicSignalSmooth, 
0x33ed68b, 0xdc75, 0x4782, 0xb3, 0xc1, 0x6a, 0xcf, 0x56, 0xc, 0x74, 0xb2);

DECLARE_INTERFACE_(IUltrasonicSignalSmooth, IUnknown)
{
	// Scale
	STDMETHOD(GetSmooth) (THIS_
		LONG *value					// get current smooth
		) PURE;
	STDMETHOD(SetSmooth) (THIS_
		LONG *value					// set current smooth
		) PURE;
	STDMETHOD(GetSmoothDefault) (THIS_
		LONG *value					// get default smooth
		) PURE;
	STDMETHOD(GetSmoothRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

	// formats
	STDMETHOD(SetSmoothFormat) (THIS_
		PDATAFORMAT pDataFormat	// set format
		) PURE;
	STDMETHOD(GetSmoothFormat) (THIS_
		PDATAFORMAT pDataFormat	// get format
		) PURE;
	STDMETHOD(GetSmoothFormatDefault) (THIS_
		PDATAFORMAT pDataFormat	// default format
		) PURE;
	STDMETHOD(EnumSmoothFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	// convert from current to specified format
	STDMETHOD(ConvertSmoothToFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
	// convert from specified to current format
	STDMETHOD(ConvertSmoothFromFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
};

// {3FC412F7-8B05-46e3-9BF4-60865CB97EFF}
DEFINE_GUID(IID_IUltrasonicSignalScale, 
0x3fc412f7, 0x8b05, 0x46e3, 0x9b, 0xf4, 0x60, 0x86, 0x5c, 0xb9, 0x7e, 0xff);

DECLARE_INTERFACE_(IUltrasonicSignalScale, IUnknown)
{
	// Scale
	STDMETHOD(GetScale) (THIS_
		LONG *value					// get current scale
		) PURE;
	STDMETHOD(SetScale) (THIS_
		LONG *value					// set current scale
		) PURE;
	STDMETHOD(GetScaleDefault) (THIS_
		LONG *value					// get default scale
		) PURE;
	STDMETHOD(GetScaleRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

	// formats
	STDMETHOD(SetScaleFormat) (THIS_
		PDATAFORMAT pDataFormat	// set format
		) PURE;
	STDMETHOD(GetScaleFormat) (THIS_
		PDATAFORMAT pDataFormat	// get format
		) PURE;
	STDMETHOD(GetScaleFormatDefault) (THIS_
		PDATAFORMAT pDataFormat	// default format
		) PURE;
	STDMETHOD(EnumScaleFormats) (THIS_
		IEnumFormats **pEnum							// enum formats
		) PURE;

	// conversions
	// convert from current to specified format
	STDMETHOD(ConvertScaleToFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
	// convert from specified to current format
	STDMETHOD(ConvertScaleFromFormat) (THIS_
		LONG *pValues,
		ULONG cValuesNum,
		PDATAFORMAT pFormat,
		ULONG *cbProcessed
		) PURE;
};

// {F188490F-425B-40fa-8559-78DE7C4E512F}
DEFINE_GUID(IID_IUltrasonicPacketSize, 
0xf188490f, 0x425b, 0x40fa, 0x85, 0x59, 0x78, 0xde, 0x7c, 0x4e, 0x51, 0x2f);
DECLARE_INTERFACE_(IUltrasonicPacketSize, IUnknown)
{
	// Scale
	STDMETHOD(GetPacketSize) (THIS_
		LONG *value					// get current scale
		) PURE;
	STDMETHOD(SetPacketSize) (THIS_
		LONG *value					// set current scale
		) PURE;
	STDMETHOD(GetPacketSizeDefault) (THIS_
		LONG *value					// get default scale
		) PURE;
	STDMETHOD(GetAvailablePacketSizes) (THIS_
		LONG* pItems,		// numer of available items
		LONG* pAvailableValues	// buffer to receive available values
		) PURE;

};

// {41C69EDA-2982-4162-ACF1-2047E8AAD9B2}
DEFINE_GUID(IID_IUltrasonicPulsesNumber, 
0x41c69eda, 0x2982, 0x4162, 0xac, 0xf1, 0x20, 0x47, 0xe8, 0xaa, 0xd9, 0xb2);
DECLARE_INTERFACE_(IUltrasonicPulsesNumber, IUnknown)
{
	STDMETHOD(GetPulsesNumber) (THIS_
		LONG *value					// get current scale
		) PURE;
	STDMETHOD(SetPulsesNumber) (THIS_
		LONG *value					// set current scale
		) PURE;
	STDMETHOD(GetPulsesNumberDefault) (THIS_
		LONG *value					// get default scale
		) PURE;
	STDMETHOD(GetPulsesNumberRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

};


// {C96E8A8E-A59D-4239-97B0-5C87C42277FF}
DEFINE_GUID(IID_IUltrasonicSpectralAvg, 
0xc96e8a8e, 0xa59d, 0x4239, 0x97, 0xb0, 0x5c, 0x87, 0xc4, 0x22, 0x77, 0xff);
DECLARE_INTERFACE_(IUltrasonicSpectralAvg, IUnknown)
{
	STDMETHOD(GetSpectralAvg) (THIS_
		LONG *value					// get current scale
		) PURE;
	STDMETHOD(SetSpectralAvg) (THIS_
		LONG *value					// set current scale
		) PURE;
	STDMETHOD(GetSpectralAvgDefault) (THIS_
		LONG *value					// get default scale
		) PURE;
	STDMETHOD(GetSpectralAvgRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;

};


// {AB5C2B73-1AEC-447e-956F-CD98C6B1C897}
DEFINE_GUID(IID_IUltrasonicSteerAngle, 
0xab5c2b73, 0x1aec, 0x447e, 0x95, 0x6f, 0xcd, 0x98, 0xc6, 0xb1, 0xc8, 0x97);

DECLARE_INTERFACE_(IUltrasonicSteerAngle, IUnknown)
{
	STDMETHOD(GetSteerAngle) (THIS_
		LONG *value					// get current angle
		) PURE;
	STDMETHOD(SetSteerAngle) (THIS_
		LONG *value					// set current angle
		) PURE;
	STDMETHOD(GetSteerAngleDefault) (THIS_
		LONG *value					// get default angle
		) PURE;
	STDMETHOD(GetAvailableSteerAngles) (THIS_
		LONG* pItems,		// numer of available items
		LONG* pAvailableAngles	// buffer to receive available angles
		) PURE;
};

// {4AED86E5-CF4F-4008-A1B9-6B8596C789AC}
DEFINE_GUID(IID_IUltrasonicAudioVolume, 
0x4aed86e5, 0xcf4f, 0x4008, 0xa1, 0xb9, 0x6b, 0x85, 0x96, 0xc7, 0x89, 0xac);
DECLARE_INTERFACE_(IUltrasonicAudioVolume, IUnknown)
{
	// Scale
	STDMETHOD(GetVolume) (THIS_
		LONG *value					// get current volume
		) PURE;
	STDMETHOD(SetVolume) (THIS_
		LONG *value					// set current volume
		) PURE;
	STDMETHOD(GetVolumeDefault) (THIS_
		LONG *value					// get default volume
		) PURE;
	STDMETHOD(GetVolumeRange) (THIS_
		VALUE_RANGE *pRange		// range of valid values
		) PURE;
};

// {4AEB7DFD-C9A4-4d1e-97B4-980112357B48}
DEFINE_GUID(IID_IUltrasonicBioeffectsIndices, 
0x4aeb7dfd, 0xc9a4, 0x4d1e, 0x97, 0xb4, 0x98, 0x1, 0x12, 0x35, 0x7b, 0x48);
DECLARE_INTERFACE_(IUltrasonicBioeffectsIndices, IUnknown)
{
	STDMETHOD(QuerySupport) (THIS_
		LONG nIndex
		) PURE;
	STDMETHOD(GetIndexValues) (THIS_
		LONG nIndex, 
		BOUNDS *pBounds
		) PURE;
};

// {7B336F1C-0DB4-466f-86BF-B5D10A42C950}
DEFINE_GUID(IID_IUsgStreamConfig, 
0x7b336f1c, 0xdb4, 0x466f, 0x86, 0xbf, 0xb5, 0xd1, 0xa, 0x42, 0xc9, 0x50);
DECLARE_INTERFACE_(IUsgStreamConfig,IUnknown)
{
	STDMETHOD(GetFormat)(THIS_
		AM_MEDIA_TYPE **pmt
		) PURE;
	STDMETHOD(GetNumberOfCapabilities)(THIS_
		int *piCount,
		int *piSize
		) PURE;
	STDMETHOD(GetFormatCaps)(THIS_
		int iIndex,
		AM_MEDIA_TYPE **pmt,
		BYTE *pSCC
		) PURE;
	STDMETHOD(SetFormat)(THIS_
		AM_MEDIA_TYPE *pmt
		) PURE;
};


///////////////////////////////////////////////////////////////////////////////////
//
//	ultrasonic filter devices categories
///////////////////////////////////////////////////////////////////////////////////

// {2F5577D7-88C1-4275-BBD2-71C6BBC3D17B}
DEFINE_GUID(CLSID_UltrasonicResearchInstrument,
0x2f5577d7, 0x88c1, 0x4275, 0xbb, 0xd2, 0x71, 0xc6, 0xbb, 0xc3, 0xd1, 0x7b);

// {5C259E00-F30A-11d6-95E3-0020ED1A49FF}
DEFINE_GUID(CLSID_UltrasonicBeamformer, 
0x5c259e00, 0xf30a, 0x11d6, 0x95, 0xe3, 0x0, 0x20, 0xed, 0x1a, 0x49, 0xff);

// {FE1D88A0-F32B-11d6-95E3-0020ED1A49FF}
DEFINE_GUID(CLSID_UltrasonicTransducer, 
0xfe1d88a0, 0xf32b, 0x11d6, 0x95, 0xe3, 0x0, 0x20, 0xed, 0x1a, 0x49, 0xff);

///////////////////////////////////////////////////////////////////////////////////
//
// property pages of ksusgcap proxy
///////////////////////////////////////////////////////////////////////////////////

// {5588F1C6-897B-40a2-AFA9-6714C4D8945E}
DEFINE_GUID(CLSID_UltrasonicTgcPropPage, 
0x5588f1c6, 0x897b, 0x40a2, 0xaf, 0xa9, 0x67, 0x14, 0xc4, 0xd8, 0x94, 0x5e);

// {33E2AB6B-261D-4bdd-A4A7-AA807E75642B}
DEFINE_GUID(CLSID_UltrasonicTfcPropPage, 
0x33e2ab6b, 0x261d, 0x4bdd, 0xa4, 0xa7, 0xaa, 0x80, 0x7e, 0x75, 0x64, 0x2b);

// {12B36776-326D-48f9-A62F-C44DE2302185}
DEFINE_GUID(CLSID_UltrasonicPulseDurPropPage, 
0x12b36776, 0x326d, 0x48f9, 0xa6, 0x2f, 0xc4, 0x4d, 0xe2, 0x30, 0x21, 0x85);

// {99A11742-38B1-468f-9418-2E8F544CF94B}
DEFINE_GUID(CLSID_UltrasonicPowerPropPage, 
0x99a11742, 0x38b1, 0x468f, 0x94, 0x18, 0x2e, 0x8f, 0x54, 0x4c, 0xf9, 0x4b);

// {9CA879D5-4583-44e7-AE66-367FF62F2616}
DEFINE_GUID(CLSID_UltrasonicFocusPropPage, 
0x9ca879d5, 0x4583, 0x44e7, 0xae, 0x66, 0x36, 0x7f, 0xf6, 0x2f, 0x26, 0x16);

// {81A34131-5C6E-4308-91E3-E4036B5BCF76}
DEFINE_GUID(CLSID_UltrasonicAgcPropPage, 
0x81a34131, 0x5c6e, 0x4308, 0x91, 0xe3, 0xe4, 0x3, 0x6b, 0x5b, 0xcf, 0x76);

// {56CC067E-CCFE-4c9e-92AC-EBFC455DAE42}
DEFINE_GUID(CLSID_UltrasonicDynRangePropPage, 
0x56cc067e, 0xccfe, 0x4c9e, 0x92, 0xac, 0xeb, 0xfc, 0x45, 0x5d, 0xae, 0x42);

// {42EF23EC-3372-4d24-AFC9-4EC6ECC3D03C}
DEFINE_GUID(CLSID_UltrasonicGainPropPage, 
0x42ef23ec, 0x3372, 0x4d24, 0xaf, 0xc9, 0x4e, 0xc6, 0xec, 0xc3, 0xd0, 0x3c);

// {846BB29B-00F3-4ab6-B922-6FE9FD19C8DE}
DEFINE_GUID(CLSID_UltrasonicProbeFrequencyPropPage, 
0x846bb29b, 0xf3, 0x4ab6, 0xb9, 0x22, 0x6f, 0xe9, 0xfd, 0x19, 0xc8, 0xde);

// {2273100D-4676-4c6f-98A6-90D6BFF2BF29}
DEFINE_GUID(CLSID_UltrasonicDeviceMiscModesPropPage, 
0x2273100d, 0x4676, 0x4c6f, 0x98, 0xa6, 0x90, 0xd6, 0xbf, 0xf2, 0xbf, 0x29);

////////////////////////////////////////////////////////////////////////////////////
//
//	USGfW objects
//
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//		Frame Averaging Filter
////////////////////////////////////////////////////////////////////////////////////

// {48DC4CF4-33AD-46d9-A052-FDB14C763F60}
DEFINE_GUID(CLSID_FrameMaxFilter, 
0x48dc4cf4, 0x33ad, 0x46d9, 0xa0, 0x52, 0xfd, 0xb1, 0x4c, 0x76, 0x3f, 0x60);

// {3E97E343-2FF0-451a-9A82-7064D98ADC26}
DEFINE_GUID(CLSID_FrameAvgFilter, 
0x3e97e343, 0x2ff0, 0x451a, 0x9a, 0x82, 0x70, 0x64, 0xd9, 0x8a, 0xdc, 0x26);

// {F8FCA6EB-9184-4cd9-8CA5-B6FC74777D0D}
DEFINE_GUID(CLSID_FrameAvgCDFilter, 
0xf8fca6eb, 0x9184, 0x4cd9, 0x8c, 0xa5, 0xb6, 0xfc, 0x74, 0x77, 0x7d, 0xd);
// IUltrasonicFrameAvg interface

// {BE71A1CD-78DF-46df-BC69-BB9D67BBC1D9}
DEFINE_GUID(IID_IUltrasonicFrameAvg, 
0xbe71a1cd, 0x78df, 0x46df, 0xbc, 0x69, 0xbb, 0x9d, 0x67, 0xbb, 0xc1, 0xd9);
DECLARE_INTERFACE_(IUltrasonicFrameAvg,IUnknown)
{
	// gain
	STDMETHOD(GetFrameAvg) (THIS_
		LONG *frameavg					// get current frameavg
		) PURE;
	STDMETHOD(SetFrameAvg) (THIS_
		LONG *frameavg					// set current frameavg
		) PURE;
	STDMETHOD(GetFrameAvgRange) (THIS_
		LONG *frameavgmin,
		LONG *frameavgmax
		) PURE;
};

// {892C5AF3-363C-4e21-A8E7-682113CC9ECB}
DEFINE_GUID(CLSID_FrameAvgCDFilter2, 
0x892c5af3, 0x363c, 0x4e21, 0xa8, 0xe7, 0x68, 0x21, 0x13, 0xcc, 0x9e, 0xcb);
// {3C8D43D6-8C74-4e78-8DB6-0A35618AB82C}
DEFINE_GUID(IID_IUltrasonicFrameAvgCD, 
0x3c8d43d6, 0x8c74, 0x4e78, 0x8d, 0xb6, 0xa, 0x35, 0x61, 0x8a, 0xb8, 0x2c);

DECLARE_INTERFACE_(IUltrasonicFrameAvgCD,IUnknown)
{
	STDMETHOD(GetFrameAvgParams) (THIS_
		LONG *persistRise,
		LONG *persistFall
		) PURE;
	STDMETHOD(SetFrameAvgParams) (THIS_
		LONG *persistRise,
		LONG *persistFall
		) PURE;
};


// guid for automatic frame averaging interface (defined for future support)

// {2F420588-6425-46d6-874E-DDE2CAF96E97}
DEFINE_GUID(IID_UltrasonicAutoFrameAvg, 
0x2f420588, 0x6425, 0x46d6, 0x87, 0x4e, 0xdd, 0xe2, 0xca, 0xf9, 0x6e, 0x97);

// Property page for frame averaging filter

// {20009569-C0C7-4bd9-A972-2A235FA8B813}
DEFINE_GUID(CLSID_UltrasonicFrameAvgPropPage, 
0x20009569, 0xc0c7, 0x4bd9, 0xa9, 0x72, 0x2a, 0x23, 0x5f, 0xa8, 0xb8, 0x13);


////////////////////////////////////////////////////////////////////////////////////
//		Frame Buffer B Filter
////////////////////////////////////////////////////////////////////////////////////


// {92EAF8D1-BA75-4e7f-AD55-CAE0EF3A6863}
DEFINE_GUID(CLSID_FrameBufFilter, 
			0x92eaf8d1, 0xba75, 0x4e7f, 0xad, 0x55, 0xca, 0xe0, 0xef, 0x3a, 0x68, 0x63 );


// IUltrasonicFrameBuf interface

// {FE87178F-9225-4842-8290-D01B40F69D68}
DEFINE_GUID(IID_IUltrasonicFrameBuf, 
			0xfe87178f, 0x9225, 0x4842, 0x82, 0x90, 0xd0, 0x1b, 0x40, 0xf6, 0x9d, 0x68 );


DECLARE_INTERFACE_(IUltrasonicFrameBuf,IUnknown)
{
	STDMETHOD(GetFrameBufSize) (THIS_
		LONG *size
		) PURE;
	STDMETHOD(SetFrameBufSize) (THIS_
		LONG *size
		) PURE;
	STDMETHOD(Freeze) (
		) PURE;
	STDMETHOD(Run) (
		) PURE;
	STDMETHOD(IsFreeze) (THIS_
		BOOL *pFreeze
		) PURE;
	STDMETHOD(PrevFrame) (
		) PURE;
	STDMETHOD(NextFrame) (
		) PURE;
	STDMETHOD(GetCurrentFrameIdx) (THIS_
		LONG *idx
		) PURE;
	STDMETHOD(SetCurrentFrameIdx) (THIS_
		LONG *idx
		) PURE;
	STDMETHOD(Clear) (
		) PURE;

};


// {49DC2FC8-812D-4933-AD78-A37685077FE3}
DEFINE_GUID(IID_IUltrasonicParallelBeamforming, 
0x49dc2fc8, 0x812d, 0x4933, 0xad, 0x78, 0xa3, 0x76, 0x85, 0x7, 0x7f, 0xe3);

DECLARE_INTERFACE_(IUltrasonicParallelBeamforming, IUnknown)
{
	STDMETHOD(GetState) (THIS_
		ULONG BeamformingType,
		ULONG *value					// get current parallel beamforming
		) PURE;
	STDMETHOD(SetState) (THIS_
		ULONG BeamformingType,
		ULONG *value					// set current parallel beamforming
		) PURE;
	STDMETHOD(GetDefaultValue) (THIS_
		ULONG BeamformingType,
		ULONG *value					// get default parallel beamforming
		) PURE;
	STDMETHOD(GetAvailableValues) (THIS_
		ULONG BeamformingType,
		LONG* pItems,		// numer of available items
		ULONG* pAvailableValues	// buffer to receive available parallel beamforming
		) PURE;
	STDMETHOD(IsBeamformingTypeSupported)( THIS_
		ULONG BeamformingType
		) PURE;
};

typedef enum tagUSPB_METHOD
{
	USPB_MODE_MULTIBEAM = 0x00,
} USPB_METHOD;


// Property page for Frame Buffer B filter

// {3F85AE06-0192-48b8-B1C1-7706FA26D2D9}
DEFINE_GUID(CLSID_UltrasonicFrameBufPropPage, 
			0x3f85ae06, 0x192, 0x48b8, 0xb1, 0xc1, 0x77, 0x6, 0xfa, 0x26, 0xd2, 0xd9 );

/////////////////////////////////////////////////////////////////////////////////////////////////
// convolution filter
/////////////////////////////////////////////////////////////////////////////////////////////////

// {5ABC1693-8367-4096-9C04-45725719280E}
DEFINE_GUID(CLSID_UsgBmodeConvolve2D, 
0x5abc1693, 0x8367, 0x4096, 0x9c, 0x4, 0x45, 0x72, 0x57, 0x19, 0x28, 0xe);

// {804468A6-3D93-4650-9B95-21F3477FE62A}
DEFINE_GUID(CLSID_UsgBmodeConvolve2DPropPage, 
0x804468a6, 0x3d93, 0x4650, 0x9b, 0x95, 0x21, 0xf3, 0x47, 0x7f, 0xe6, 0x2a);


// interface declaration

// {C9A6B146-55CF-48b4-A055-604C09F80BC7}
DEFINE_GUID(IID_IUsgConvolve2D, 
0xc9a6b146, 0x55cf, 0x48b4, 0xa0, 0x55, 0x60, 0x4c, 0x9, 0xf8, 0xb, 0xc7);

DECLARE_INTERFACE_(IUsgConvolve2D,IUnknown)
{
	STDMETHOD(SetMatrix) (THIS_
		SIZE* pMatrixSize,
		POINT* pMatrixCenter,
		LONG* pMatrixData,
		ULONG ulRShift
		) PURE;
	STDMETHOD(SetROI) (THIS_
		RECT *pROI
		) PURE;
	STDMETHOD(EnableConvolver) (THIS_
		BOOL fEnable
		) PURE;
	STDMETHOD(IsConvolverEnabled) (THIS_
		BOOL *pEnabled
		) PURE;
	STDMETHOD(GetMatrixSize) (THIS_
		SIZE* pSize
		) PURE;
	STDMETHOD(GetMatrixCenter) (THIS_
		POINT* pPoint
		) PURE;
	STDMETHOD(GetRShift) (THIS_
		ULONG* pRShift
		) PURE;
	STDMETHOD(GetMatrixData) (THIS_
		ULONG uBufferSize,
		LONG* pBuffer
		) PURE;
	STDMETHOD(GetROI) (THIS_
		RECT *pROI
		) PURE;
};


////////////////////////////////////////////////////////////////////////////
//   rejection filter
////////////////////////////////////////////////////////////////////////////

// {1755A40C-A9B0-4ee5-BCD7-BD9D70849C3D}
DEFINE_GUID(CLSID_UsgRejection, 
0x1755a40c, 0xa9b0, 0x4ee5, 0xbc, 0xd7, 0xbd, 0x9d, 0x70, 0x84, 0x9c, 0x3d);

// {A6B03056-613A-4856-A6D6-7192C41E90DA}
DEFINE_GUID(CLSID_UsgRejection2,
0xa6b03056, 0x613a, 0x4856, 0xa6, 0xd6, 0x71, 0x92, 0xc4, 0x1e, 0x90, 0xda);


// {E5119278-DEE8-47c8-AB46-4F6D92AEF6E7}
DEFINE_GUID(CLSID_UsgRejectionPropPage, 
0xe5119278, 0xdee8, 0x47c8, 0xab, 0x46, 0x4f, 0x6d, 0x92, 0xae, 0xf6, 0xe7);

// объ€вление интерфейса

// {055E0DC4-3B06-44ce-B253-5AD3F55B85FE}
DEFINE_GUID(IID_IUsgRejection, 
0x55e0dc4, 0x3b06, 0x44ce, 0xb2, 0x53, 0x5a, 0xd3, 0xf5, 0x5b, 0x85, 0xfe);

DECLARE_INTERFACE_(IUsgRejection,IUnknown)
{
	STDMETHOD(EnableRejection) (THIS_
		BOOL fEnable
		) PURE;
	STDMETHOD(IsRejectionEnabled) (THIS_
		BOOL *pEnabled
		) PURE;
	STDMETHOD(SetRejectionUpperBound) (THIS_
		LONG RejectionBound
		) PURE;
	STDMETHOD(GetRejectionUpperBound) (THIS_
		LONG* pRejectionBound
		) PURE;
	STDMETHOD(SetRejectionLowerBound) (THIS_
		LONG RejectionBound
		) PURE;
	STDMETHOD(GetRejectionLowerBound) (THIS_
		LONG* pRejectionBound
		) PURE;
	STDMETHOD(GetRejectionRange)(THIS_
		LONG* pMin,
		LONG* pMax
		) PURE;
};

////////////////////////////////////////////////////////////////////////////
// auto histogram adjustment
////////////////////////////////////////////////////////////////////////////

// {BCAD232C-67DB-45ac-9315-A3EE944835CE}
DEFINE_GUID(CLSID_UsgAutoHistogram, 
0xbcad232c, 0x67db, 0x45ac, 0x93, 0x15, 0xa3, 0xee, 0x94, 0x48, 0x35, 0xce);

// {5F2340AD-9C3C-4bf8-8A1A-2D0706E444A7}
DEFINE_GUID(CLSID_UsgAutoHistogramPropPage, 
0x5f2340ad, 0x9c3c, 0x4bf8, 0x8a, 0x1a, 0x2d, 0x7, 0x6, 0xe4, 0x44, 0xa7);

// объ€вление интерфейса

// {2A978616-B327-453a-B632-FC273D9895C5}
DEFINE_GUID(IID_IUsgAutoHistogram, 
0x2a978616, 0xb327, 0x453a, 0xb6, 0x32, 0xfc, 0x27, 0x3d, 0x98, 0x95, 0xc5);

DECLARE_INTERFACE_(IUsgAutoHistogram,IUnknown)
{
	STDMETHOD(Enable) (THIS_
		BOOL fEnable
		) PURE;
	STDMETHOD(IsEnabled) (THIS_
		BOOL *pEnabled
		) PURE;
	STDMETHOD(SetROI) (THIS_
		RECT *pROI
		) PURE;
	STDMETHOD(GetROI) (THIS_
		RECT *pROI
		) PURE;
};

//////////////////////////////////////////////////////////////////////////
// IUltrasonicDopplerColorMap interface
//////////////////////////////////////////////////////////////////////////

// {50BEFC75-34D9-4125-AAA2-25E8257A7B06}
DEFINE_GUID(IID_IUltrasonicDopplerColorMap, 
	0x50befc75, 0x34d9, 0x4125, 0xaa, 0xa2, 0x25, 0xe8, 0x25, 0x7a, 0x7b, 0x6);
DECLARE_INTERFACE_(IUltrasonicDopplerColorMap, IUnknown)
{
	STDMETHOD(SetColorMap)(THIS_
		PALETTEENTRY* pFirstTable,			// array that receives entries
		ULONG cFirstTableSize,				// number of entries to retrieve
		PALETTEENTRY* pSecondTable,			// array that receives entries
		ULONG cSecondTableSize				// number of entries to retrieve
		) PURE;
	STDMETHOD(GetPaletteEntries)(THIS_
		ULONG iStart,				// first entry to retrieve
		ULONG cEntries,				// number of entries to retrieve
		PALETTEENTRY* ppe			// array that receives entries
		) PURE;
	STDMETHOD(SetBaseLine)(THIS_
		LONG baseLine
		) PURE;
	STDMETHOD_(LONG, GetBaseLine)(THIS_
		) PURE;
};

// {BD564930-8D37-454f-BD3B-DC7AE855C35C}
DEFINE_GUID(IID_IDuplexColorPriority, 
			0xbd564930, 0x8d37, 0x454f, 0xbd, 0x3b, 0xdc, 0x7a, 0xe8, 0x55, 0xc3, 0x5c);
DECLARE_INTERFACE_(IDuplexColorPriority, IUnknown)
{
	STDMETHOD_(LONG, GetColorPriorityLevel) (THIS_
		) PURE;
	STDMETHOD(SetColorPriorityLevel) (THIS_
		LONG priority
		) PURE;
	STDMETHOD_(BOOL, PriorityModeB) (THIS_
		) PURE;
	STDMETHOD(SetPriorityModeB)(THIS_
		BOOL priorityB
		) PURE;
};

// {0890AC2B-2B35-4aef-A0F3-45E94F29A766}
DEFINE_GUID(IID_IDopplerColorThreshold, 
0x890ac2b, 0x2b35, 0x4aef, 0xa0, 0xf3, 0x45, 0xe9, 0x4f, 0x29, 0xa7, 0x66);
DECLARE_INTERFACE_(IDopplerColorThreshold, IUnknown)
{
	STDMETHOD_(LONG, GetThresholdLevel)(THIS_
		) PURE;
	STDMETHOD(SetThresholdLevel)(THIS_
		LONG threshold
		) PURE;
};

// {1DD0E0A9-CAC1-4096-9FC7-4274662B56D2}
DEFINE_GUID(IID_IDuplexColorTransparency,
			0x1dd0e0a9, 0xcac1, 0x4096, 0x9f, 0xc7, 0x42, 0x74, 0x66, 0x2b, 0x56, 0xd2);

DECLARE_INTERFACE_(IDuplexColorTransparency, IUnknown)
{
	STDMETHOD_(ULONG, GetTransparencyMode) (THIS_
		) PURE;
	STDMETHOD(SetTransparencyMode) (THIS_
		ULONG mode) PURE;
	STDMETHOD_(LONG, GetTransparencyLevel) (THIS_
		) PURE;
	STDMETHOD(SetTransparencyLevel) (THIS_
		LONG level) PURE;
};

////////////////////////////////////////////////////////////////////////////
//	UsgfW Video Renderer
////////////////////////////////////////////////////////////////////////////

// {7BB063C3-146F-4362-B11A-200012B94F39}
//DEFINE_GUID(CLSID_UsgfwVideoRenderer, 
//0x7bb063c3, 0x146f, 0x4362, 0xb1, 0x1a, 0x20, 0x0, 0x12, 0xb9, 0x4f, 0x39);


// {2B90F7A8-EE32-4f9c-84DF-0F890827A772}
DEFINE_GUID(CLSID_ContextVisionFilter, 
0x2b90f7a8, 0xee32, 0x4f9c, 0x84, 0xdf, 0xf, 0x89, 0x8, 0x27, 0xa7, 0x72);


// {61B11B81-2A34-47ed-822A-7B296505A99D}
DEFINE_GUID(CLSID_ContextVisionFilterPropPage, 
0x61b11b81, 0x2a34, 0x47ed, 0x82, 0x2a, 0x7b, 0x29, 0x65, 0x5, 0xa9, 0x9d);

// {569282B3-5EFC-4428-92CF-8A5803C1B04F}
DEFINE_GUID(CLSID_SpeckleReductionFilterCV4,
0x569282b3, 0x5efc, 0x4428, 0x92, 0xcf, 0x8a, 0x58, 0x3, 0xc1, 0xb0, 0x4f);


// {78765DD1-AA73-4c06-89F1-069375AF2148}
DEFINE_GUID(CLSID_SpeckleReductionFilter, 
0x78765dd1, 0xaa73, 0x4c06, 0x89, 0xf1, 0x6, 0x93, 0x75, 0xaf, 0x21, 0x48);


// {448E7033-A15A-4de0-8FBE-189CF808C0AC}
DEFINE_GUID(CLSID_SpeckleReductionFilterPropPage, 
0x448e7033, 0xa15a, 0x4de0, 0x8f, 0xbe, 0x18, 0x9c, 0xf8, 0x8, 0xc0, 0xac);

// {A68A0B1A-D5C3-420e-A3F0-0B653A764204}
DEFINE_GUID(CLSID_SpeckleReductionCDFilter, 
0xa68a0b1a, 0xd5c3, 0x420e, 0xa3, 0xf0, 0xb, 0x65, 0x3a, 0x76, 0x42, 0x4);

// {E519639F-7219-4d29-A233-DD362755D0D4}
DEFINE_GUID(CLSID_SpeckleReductionCDFilterPropPage, 
0xe519639f, 0x7219, 0x4d29, 0xa2, 0x33, 0xdd, 0x36, 0x27, 0x55, 0xd0, 0xd4);

// {11E683E4-EB64-4223-B7D1-6019B3A30BC1}
DEFINE_GUID(CLSID_SpeckleReductionQvFilter, 
0x11e683e4, 0xeb64, 0x4223, 0xb7, 0xd1, 0x60, 0x19, 0xb3, 0xa3, 0xb, 0xc1);


//////////////////////////////////////////////////////////////////////////
// USGFW stream buffer interfaces
//////////////////////////////////////////////////////////////////////////

// {DD8781E5-3E7F-43fe-B3A2-3FB50FE1D4B1}
DEFINE_GUID(CLSID_UsgStreamBufferSink, 
0xdd8781e5, 0x3e7f, 0x43fe, 0xb3, 0xa2, 0x3f, 0xb5, 0xf, 0xe1, 0xd4, 0xb1);

// {60144AB9-53AD-4a14-AE80-DD0B06666993}
DEFINE_GUID(CLSID_UsgStreamBufferSource, 
0x60144ab9, 0x53ad, 0x4a14, 0xae, 0x80, 0xdd, 0xb, 0x6, 0x66, 0x69, 0x93);

// {BD955271-9B5A-46c6-81E3-052F7ECCAB33}
DEFINE_GUID(IID_IUsgStreamBufferSink, 
0xbd955271, 0x9b5a, 0x46c6, 0x81, 0xe3, 0x5, 0x2f, 0x7e, 0xcc, 0xab, 0x33);

DECLARE_INTERFACE_(IUsgStreamBufferSink, IUnknown)
{
	STDMETHOD(GetBufferInfo)(THIS_
		LONG_PTR* pBufferSize,
		LONG_PTR* pActualData,
		LONG_PTR* pWritePos
		) PURE;
	STDMETHOD(Reset) (THIS_
		void
		) PURE;
	STDMETHOD(SetBufferSize) (THIS_
		LONG_PTR cbBufferSize
		) PURE;
	STDMETHOD(SaveData) (THIS_
		BSTR pFileName
		) PURE;
};

/*
// {8A476C5F-C74C-4EEE-B785-E18D19904000}
DEFINE_GUID(IID_IUsgStreamBufferSink2,
0x8a476c5f, 0xc74c, 0x4eee, 0xb7, 0x85, 0xe1, 0x8d, 0x19, 0x90, 0x40, 0x0);
DECLARE_INTERFACE_(IUsgStreamBufferSink2, IUsgStreamBufferSink)
{
	STDMETHOD(GetBufferInfo)(THIS_
		LONG64 * pBufferSize,
		LONG64 * pActualData,
		LONG64 * pWritePos
		) PURE;
	STDMETHOD(SetBufferSize) (THIS_
		LONG64 cbBufferSize
		) PURE;
};
*/

// {25427DDD-F7D2-49b9-989C-663001D8BADE}
DEFINE_GUID(IID_IUsgStreamBufferSource, 
0x25427ddd, 0xf7d2, 0x49b9, 0x98, 0x9c, 0x66, 0x30, 0x1, 0xd8, 0xba, 0xde);

DECLARE_INTERFACE_(IUsgStreamBufferSource, IUnknown)
{
	STDMETHOD(SetStreamSink)(THIS_
		IUsgStreamBufferSink* pSink
		) PURE;
	STDMETHOD(SetFilterCookie) (THIS_
		LONG_PTR nCookie
		) PURE;
	STDMETHOD_(LONG_PTR, GetFilterCookie) (THIS_
		void
		) PURE;
};

// {32D3165A-C368-45ab-ABFC-68630074D5C6}
DEFINE_GUID(IID_IUsgStreamBufferMediaSeeking, 
0x32d3165a, 0xc368, 0x45ab, 0xab, 0xfc, 0x68, 0x63, 0x0, 0x74, 0xd5, 0xc6);

DECLARE_INTERFACE_(IUsgStreamBufferMediaSeeking, IMediaSeeking)
{
	STDMETHOD(SetHoldRange)(THIS_
		LONGLONG *pFrom,
		LONGLONG *pTo
		) PURE;
	STDMETHOD(GetHoldRange)(THIS_
		LONGLONG *pFrom,
		LONGLONG *pTo
		) PURE;
	STDMETHOD(GetContinuousInterval)(THIS_
		LONGLONG position,
		LONGLONG* pStart,
		LONGLONG* pEnd
		) PURE;
	STDMETHOD(GetShiftedPosition)(THIS_
		LONGLONG from,
		LONGLONG shift,
		const GUID* pTimeFormat,
		LONGLONG* pPos
		) PURE;
	STDMETHOD(SetOutputWidth)(THIS_
		LONG *pWidth
		) PURE;
};


// {20E78EDC-E338-4678-8F8C-7EBE1A43B3CA}
DEFINE_GUID(CLSID_UsgSpatialCDFilter, 
0x20e78edc, 0xe338, 0x4678, 0x8f, 0x8c, 0x7e, 0xbe, 0x1a, 0x43, 0xb3, 0xca);

// {67D031EC-8D79-4aba-8EBC-66C2EE1C8EDF}
DEFINE_GUID(IID_IUsgSpatial, 
0x67d031ec, 0x8d79, 0x4aba, 0x8e, 0xbc, 0x66, 0xc2, 0xee, 0x1c, 0x8e, 0xdf);

DECLARE_INTERFACE_(IUsgSpatial,IUnknown)
{
	STDMETHOD(SetMatrix) (THIS_
		SIZE* pMatrixSize,
		POINT* pMatrixCenter
		) PURE;
	STDMETHOD(SetROI) (THIS_
		RECT *pROI
		) PURE;
	STDMETHOD(Enable) (THIS_
		BOOL fEnable
		) PURE;
	STDMETHOD_(BOOL, IsEnabled) (THIS_
		) PURE;
	STDMETHOD(GetMatrixSize) (THIS_
		SIZE* pSize
		) PURE;
	STDMETHOD(GetMatrixCenter) (THIS_
		POINT* pPoint
		) PURE;
	STDMETHOD(GetROI) (THIS_
		RECT *pROI
		) PURE;
};

// {B2F17074-BBC5-4cfb-83B4-6B9B88ED9B45}
DEFINE_GUID(CLSID_FlashSuppressionPW, 
0xb2f17074, 0xbbc5, 0x4cfb, 0x83, 0xb4, 0x6b, 0x9b, 0x88, 0xed, 0x9b, 0x45);

// {1733280B-7E44-4048-B698-4B429FD1E82C}
DEFINE_GUID(IID_IFlashSuppressionPW, 
0x1733280b, 0x7e44, 0x4048, 0xb6, 0x98, 0x4b, 0x42, 0x9f, 0xd1, 0xe8, 0x2c);

DECLARE_INTERFACE_(IFlashSuppressionPW,IUnknown)
{
	STDMETHOD(SetParameters) (THIS_
		int nStartIndex,
		int nReference
		) PURE;
	STDMETHOD(Enable) (THIS_
		BOOL fEnable
		) PURE;
	STDMETHOD_(BOOL, IsEnabled) (THIS_
		) PURE;
	STDMETHOD_(int, GetStartIndex) (THIS_
		) PURE;
	STDMETHOD_(int, GetReference) (THIS_
		) PURE;
};

// {6AA16A3E-ADAE-4fbb-A5CC-33718E9C5D99}
DEFINE_GUID(CLSID_AudioLowPassFilter, 
			0x6aa16a3e, 0xadae, 0x4fbb, 0xa5, 0xcc, 0x33, 0x71, 0x8e, 0x9c, 0x5d, 0x99);

// {7131C6AE-9947-4bc7-9C49-74E1DD62AB03}
DEFINE_GUID(IID_IAudioLowPassFilter, 
			0x7131c6ae, 0x9947, 0x4bc7, 0x9c, 0x49, 0x74, 0xe1, 0xdd, 0x62, 0xab, 0x3);

DECLARE_INTERFACE_(IAudioLowPassFilter,IUnknown)
{
	STDMETHOD(SetFrequency) (THIS_
		int nFrequency
		) PURE;
	STDMETHOD_(int, GetFrequency) (THIS_
		) PURE;
};

//
// PW-mode scan-converter filter class Id
//

// {B7757BA3-882A-46e3-B764-8F27A503B6EC}
DEFINE_GUID(CLSID_ScanConverterModePW, 
0xb7757ba3, 0x882a, 0x46e3, 0xb7, 0x64, 0x8f, 0x27, 0xa5, 0x3, 0xb6, 0xec);

//
// CM-mode scan-converter filter class Id
//
// {B6B111B1-2A8D-47dd-BF90-CBE282575BFE}
DEFINE_GUID(CLSID_ScanConverterModeCM, 
0xb6b111b1, 0x2a8d, 0x47dd, 0xbf, 0x90, 0xcb, 0xe2, 0x82, 0x57, 0x5b, 0xfe);


// {AD8F939C-689E-4efa-B090-DC944274C7AC}
DEFINE_GUID(IID_IUsgfwScanConverterPluginCB, 
0xad8f939c, 0x689e, 0x4efa, 0xb0, 0x90, 0xdc, 0x94, 0x42, 0x74, 0xc7, 0xac);

// ScanConverter plugin interfaces
DECLARE_INTERFACE_(IUsgfwScanConverterPluginCB, IUnknown)
{
	// receives pointers to input and output media samples
	STDMETHOD(SampleCB) (THIS_
		IMediaSample *pSampleIn,
		IMediaSample *pSampleOut,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) PURE;
	// receives pointers to input and output sample buffers
	STDMETHOD(BufferCB) (THIS_
		PBYTE pBufferIn,
		int nInBufferLen,
		PBYTE pBufferOut,
		int nOutBufferLen,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) PURE;
	// receives pointers to input and intermediate sample buffers
	STDMETHOD(InInterimBufferCB) (THIS_
		PBYTE pBufferIn,
		int nInBufferLen,
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) PURE;
	// receves pointers to input media sample and intermediatesample buffer
	STDMETHOD(InInterimSampleCB) (THIS_
		IMediaSample *pSampleIn,
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) PURE;
	// receives pointers to output and intermediate sample buffers
	STDMETHOD(InterimOutBufferCB) (THIS_
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		PBYTE pBufferOut,
		int nOutBufferLen,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) PURE;
	// receives pointers to output media sample and intermediate sample buffer
	STDMETHOD(InterimOutSampleCB) (THIS_
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		IMediaSample *pSampleIn,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) PURE;

	// receives conversion parameter change pin index
	// if parameter is negative parameter was changed by some filter interface
	STDMETHOD(ParameterCB) (THIS_
		int nPin
		) PURE;
};


// {5B5DE697-183C-4b76-9DFB-4DB058C73E75}
DEFINE_GUID(IID_IUsgfwScanConverterPlugin, 
0x5b5de697, 0x183c, 0x4b76, 0x9d, 0xfb, 0x4d, 0xb0, 0x58, 0xc7, 0x3e, 0x75);

typedef enum tagUSCP_METHODTOCALL
{
	USPC_SAMPLE_INPUT_OUTPUT = 0x01,
	USPC_BUFFER_INPUT_OUTPUT = 0x02,
	USPC_SAMPLE_INPUT_INTERIM = 0x04,
	USPC_BUFFER_INPUT_INTERIM = 0x08,
	USPC_SAMPLE_INTERIM_OUTPUT = 0x10,
	USPC_BUFFER_INTERIM_OUTPUT = 0x20,
} USPC_METHODTOCALL;

DECLARE_INTERFACE_(IUsgfwScanConverterPlugin, IUnknown)
{
	STDMETHOD(SetCallback)(THIS_
		IUsgfwScanConverterPluginCB *pPlugin,
		ULONG WhichMethodToCallback
		) PURE;
	STDMETHOD(GetConnectedMediaType) (THIS_
		int nPinId,
		AM_MEDIA_TYPE *pType
		) PURE;
};

// {B443C930-EC10-4900-B7FC-4F49ADDE5D3D}
DEFINE_GUID(IID_IUsgfwScanConverterPlugin2, 
0xb443c930, 0xec10, 0x4900, 0xb7, 0xfc, 0x4f, 0x49, 0xad, 0xde, 0x5d, 0x3d);


DECLARE_INTERFACE_(IUsgfwScanConverterPlugin2, IUsgfwScanConverterPlugin)
{
	STDMETHOD(SetCallback2)(THIS_
		IUsgfwScanConverterPluginCB *pPlugin,
		ULONG WhichMethodToCallback,
		int nInputPin,
		int nOutputPin
		) PURE;
};


// {2BE7113D-D403-4db5-8CFF-364168C90030}
DEFINE_GUID(IID_IUsgfwScanConverterPluginCB2, 
0x2be7113d, 0xd403, 0x4db5, 0x8c, 0xff, 0x36, 0x41, 0x68, 0xc9, 0x0, 0x30);

DECLARE_INTERFACE_(IUsgfwScanConverterPluginCB2, IUnknown)
{
	// method is called with input sample as parameter.
	// method is called just after sample is received into scan-converter
	STDMETHOD(InputSampleCB)(THIS_
		IMediaSample* pSample,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with input buffer as parameter.
	// method is called just after sample is received into scan-converter
	STDMETHOD(InputBufferCB)(THIS_
		PBYTE pBuffer,
		LONG nBufferSize,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with interim buffer as parameter.
	// method is called just after interim data is created from input data in scan-converter
	STDMETHOD(InterimBufferCB)(THIS_
		PBYTE pBuffer,
		LONG nBufferSize,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with output sample as parameter.
	// method is called just before output sample will be passed to downstream filter
	STDMETHOD(OutputSampleCB)(THIS_
		IMediaSample* pSample,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with output buffer as parameter.
	// method is called just before output data will be passed to downstream filter
	STDMETHOD(OutputBufferCB)(THIS_
		PBYTE pBuffer,
		LONG nBufferSize,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with input and output samples specified
	// method is called just before output sample will be passed to downstream filter
	STDMETHOD(SampleCB) (THIS_
		IMediaSample *pSampleIn,
		IMediaSample *pSampleOut,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with input and output buffers specified as parameters
	// method is called just before output sample will be passed to downstream filter
	STDMETHOD(BufferCB) (THIS_
		PBYTE pBufferIn,
		int nInBufferLen,
		PBYTE pBufferOut,
		int nOutBufferLen,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with pointers to input and intermediate buffers specified as parameters
	// method is called just after data is processed from input buffer into the intermediate buffer
	STDMETHOD(InInterimBufferCB) (THIS_
		PBYTE pBufferIn,
		int nInBufferLen,
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with pointers to input sample and intermediate buffer specified as parameters
	// method is called just after data is processed from input sample into the intermediate buffer
	STDMETHOD(InInterimSampleCB) (THIS_
		IMediaSample *pSampleIn,
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with pointers to intermediate buffer and output buffer specified as parameters
	// method is called just before output data buffer will be passed to downstream filter
	STDMETHOD(InterimOutBufferCB) (THIS_
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		PBYTE pBufferOut,
		int nOutBufferLen,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with pointers to intermediate buffer and output sample specified as parameters
	// method is called just before output sample will be passed to downstream filter
	STDMETHOD(InterimOutSampleCB) (THIS_
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		IMediaSample *pSampleOut,
		LONG_PTR Param1,
		LONG_PTR Param2,
		LONG_PTR Param3,
		LONG_PTR Param4
		) PURE;
	// method is called with new media type specified as parameter
	// method is called before callbacks with new format data be invoked
	STDMETHOD(MediaTypeCB) (THIS_
		AM_MEDIA_TYPE *pMediaType,
		BOOL bOutputPin
		) PURE;
	// method is called when some scan-conversion parameter was changed
	STDMETHOD(ParameterCB)(THIS_
		void
		) PURE;

};

// {CFD439AB-323E-46e3-8C31-E05E83F450C9}
DEFINE_GUID(IID_IUsgfwScanConverterPlugin4, 
0xcfd439ab, 0x323e, 0x46e3, 0x8c, 0x31, 0xe0, 0x5e, 0x83, 0xf4, 0x50, 0xc9);

typedef enum tagUSCP_METHODTOCALL2
{
	USPC2_SAMPLE_INPUT = 0x01,
	USPC2_BUFFER_INPUT = 0x02,
	USPC2_INTERIM = 0x04,
	USPC2_BUFFER_OUTPUT = 0x08,
	USPC2_SAMPLE_OUTPUT = 0x10,
	USPC2_SAMPLE_INPUT_OUTPUT = 0x20,
	USPC2_BUFFER_INPUT_OUTPUT = 0x40,
	USPC2_BUFFER_INPUT_INTERIM = 0x80,
	USPC2_SAMPLE_INPUT_INTERIM = 0x100,
	USPC2_INTERIM_BUFFER_OUTPUT = 0x200,
	USPC2_INTERIM_SAMPLE_OUTPUT = 0x400,
	USPC2_MEDIA_TYPE = 0x800,
	USPC2_PARAMETER = 0x1000
} USPC_METHODTOCALL2;

DECLARE_INTERFACE_(IUsgfwScanConverterPlugin4, IUnknown)
{
	// method adds callback specified into the scan-converter
	// if callback object already exists in scan-converter
	// new parameters are accepted to this callback object
	STDMETHOD(AddCallback)(THIS_
		IUsgfwScanConverterPluginCB2 *pPlugin,
		ULONG WhichMethodToCallback,
		int nInputPin,
		int nOutputPin
		) PURE;
	// method removes scan-converter specified from scan-converter
	STDMETHOD(RemoveCallback) (THIS_
		IUsgfwScanConverterPluginCB2 *pPlugin
		) PURE;
	// method retrieves media type of specified scan-converter pin
	STDMETHOD(GetCurrentMediaType) (THIS_
		int nPinId,
		AM_MEDIA_TYPE *pType
		) PURE;
	// method returns callback method flags that actually are selected by scan-converter for
	// callback object specified
	STDMETHOD_(ULONG,GetCallbackMethods)(THIS_
		IUsgfwScanConverterPluginCB2 *pPlugin
		) PURE;
	// method returns flags of supported callbacks of the scan-converter
	STDMETHOD_(ULONG,GetCallbackCapabilities)(THIS_
		void
		) PURE;

};


// {4D791AEB-DAF4-4543-9D99-4D19791EAFF5}
DEFINE_GUID(IID_IUsgfwScanConverterPlugin3, 
0x4d791aeb, 0xdaf4, 0x4543, 0x9d, 0x99, 0x4d, 0x19, 0x79, 0x1e, 0xaf, 0xf5);

DECLARE_INTERFACE_(IUsgfwScanConverterPlugin3, IUsgfwScanConverterPlugin)
{
	STDMETHOD(GetScanConverterObj)(THIS_
		IUnknown **pObj
		) PURE;
};


//
// this interface is declared to implement management of several plugins connected to single scan-converter object
//
// {7DFFE08A-FC9D-4166-89AB-C4E73BB303E4}
DEFINE_GUID(IID_IUsgfwScanConverterPlugin5, 
0x7dffe08a, 0xfc9d, 0x4166, 0x89, 0xab, 0xc4, 0xe7, 0x3b, 0xb3, 0x3, 0xe4);

DECLARE_INTERFACE_(IUsgfwScanConverterPlugin5, IUnknown)
{
	// method adds callback specified into the scan-converter
	// if callback object already exists in scan-converter
	// new parameters are accepted to this callback object
	STDMETHOD(AddCallback5)(THIS_
		IUsgfwScanConverterPluginCB *pPlugin,
		ULONG WhichMethodToCallback,
		int nInputPin,
		int nOutputPin
		) PURE;
	// method removes scan-converter specified from scan-converter
	STDMETHOD(RemoveCallback5) (THIS_
		IUsgfwScanConverterPluginCB *pPlugin
		) PURE;
	// method retrieves media type of specified scan-converter pin
	STDMETHOD(GetCurrentMediaType5) (THIS_
		int nPinId,
		AM_MEDIA_TYPE *pType
		) PURE;
	// method returns callback method flags that actually are selected by scan-converter for
	// callback object specified
	STDMETHOD_(ULONG,GetCallbackMethods5)(THIS_
		IUsgfwScanConverterPluginCB *pPlugin
		) PURE;
	// method returns flags of supported callbacks of the scan-converter
	STDMETHOD_(ULONG,GetCallbackCapabilities5)(THIS_
		void
		) PURE;

};


// {85EDA278-9D70-46cf-91A4-C41237616776}
DEFINE_GUID(CLSID_DynamicRangePW, 
0x85eda278, 0x9d70, 0x46cf, 0x91, 0xa4, 0xc4, 0x12, 0x37, 0x61, 0x67, 0x76);

// temprary GUID for file writer
// {CD616AC4-F89D-4d97-A126-F0A03A26C7DD}
DEFINE_GUID(CLSID_UsgUdiCoder, 
0xcd616ac4, 0xf89d, 0x4d97, 0xa1, 0x26, 0xf0, 0xa0, 0x3a, 0x26, 0xc7, 0xdd);

// {AB31C461-E67D-4173-9B73-60C72B756A0C}
DEFINE_GUID(CLSID_UsgUdiDecoder, 
0xab31c461, 0xe67d, 0x4173, 0x9b, 0x73, 0x60, 0xc7, 0x2b, 0x75, 0x6a, 0xc);

// {A76BE44A-F733-4df0-993B-89D5CA4E8AD1}
DEFINE_GUID(CLSID_ColorDopplerGainAnalize, 
0xa76be44a, 0xf733, 0x4df0, 0x99, 0x3b, 0x89, 0xd5, 0xca, 0x4e, 0x8a, 0xd1);

// {070C452B-B114-410c-94FF-79E226B4CB09}
DEFINE_GUID(IID_IColorDopplerGainAnalizeCB, 
			0x70c452b, 0xb114, 0x410c, 0x94, 0xff, 0x79, 0xe2, 0x26, 0xb4, 0xcb, 0x9);
DECLARE_INTERFACE_(IColorDopplerGainAnalizeCB, IUnknown)
{
	STDMETHOD(OnAnalizeComplete)(THIS_
		int nNewGain) PURE;
};

// {0D662EDB-D355-4110-8601-1EEBECD5E94A}
DEFINE_GUID(IID_IColorDopplerGainAnalize, 
0xd662edb, 0xd355, 0x4110, 0x86, 0x1, 0x1e, 0xeb, 0xec, 0xd5, 0xe9, 0x4a);

DECLARE_INTERFACE_(IColorDopplerGainAnalize, IUnknown)
{
	STDMETHOD(SetCallback)(THIS_
		IColorDopplerGainAnalizeCB* pCallback,
		DWORD dwCallbackFlags) PURE;

	STDMETHOD(Analize)(THIS_
		int nFramesToSkip,
		int nCurrentGain
		) PURE;

	STDMETHOD(GetAnalizeResult)( THIS_
		int *pNewGain) PURE;
};

// {C9B7170B-457C-403f-87FF-CCF14ADAA7BF}
DEFINE_GUID(IID_IUsgfwHardwareAcceleration, 
0xc9b7170b, 0x457c, 0x403f, 0x87, 0xff, 0xcc, 0xf1, 0x4a, 0xda, 0xa7, 0xbf);

DECLARE_INTERFACE_(IUsgfwHardwareAcceleration, IUnknown)
{
	// Probe frequency
	STDMETHOD_(ULONG, GetAcceleration) (THIS_
		void) PURE;
	STDMETHOD(SetAcceleration) (THIS_
		ULONG nAccelerationLevel
		) PURE;
	STDMETHOD_(ULONG, GetAccelerationDefault) (THIS_
		void) PURE;
	// methods to get exact values of supported
	// accelerations
	STDMETHOD_(ULONG, GetAccelerationsNumber)(THIS_
		void
		) PURE;
	STDMETHOD(GetAccelerationValues)(THIS_
		LONG nIndexFirst, 
		LONG nEntries, 
		ULONG* pValues,
		LONG* cFetched
		) PURE;
};



// {C5601F36-4EE1-4fff-9BFD-72D6BE06384B}
DEFINE_GUID(IID_IUsgfwRendererPluginCB, 
			0xc5601f36, 0x4ee1, 0x4fff, 0x9b, 0xfd, 0x72, 0xd6, 0xbe, 0x6, 0x38, 0x4b);

DECLARE_INTERFACE_(IUsgfwRendererPluginCB, IUnknown)
{
	STDMETHOD(FrameRenderCB)(THIS_
		LPBYTE pImageData,
		LONG nImageWidth,
		LONG nImageHeight,
		LONG nImagePitch,
		LONG nColorDepth
		) PURE;
};

// {A70236F1-DF57-4d47-8D65-8ED3B1FF531D}
DEFINE_GUID(IID_IUsgfwRendererPlugin, 
			0xa70236f1, 0xdf57, 0x4d47, 0x8d, 0x65, 0x8e, 0xd3, 0xb1, 0xff, 0x53, 0x1d);

DECLARE_INTERFACE_(IUsgfwRendererPlugin, IUnknown)
{
	STDMETHOD(SetCallback)(THIS_
		IUsgfwRendererPluginCB *pPlugin,
		DWORD dwParam 
		) PURE;
};

// {7809BE2A-6995-44f4-95A0-3380B6818363}
DEFINE_GUID(IID_ISpeckleReductionCtrl,
	0x7809be2a, 0x6995, 0x44f4, 0x95, 0xa0, 0x33, 0x80, 0xb6, 0x81, 0x83, 0x63);
DECLARE_INTERFACE_(ISpeckleReductionCtrl, IUnknown)
{
	STDMETHOD(SetParameterFile) (THIS_
		BSTR pFileName
		) PURE;
	STDMETHOD(GetMaxSettingsNum) (THIS_
		int* pSettingsNum
		) PURE;
	STDMETHOD(GetCurrentSetting) (THIS_
		int* pSetting
		) PURE;
	STDMETHOD(SetCurrentSetting) (THIS_
		int nSetting
		) PURE;
	STDMETHOD(GetParameterFile) (THIS_
		LPBSTR ppFileName
		) PURE;
	STDMETHOD(EnableProcessing) (THIS_
		BOOL fEnable
		) PURE;
	STDMETHOD_(BOOL, IsProcessingEnabled)(THIS_
		) PURE;
};

// {820FAA24-BBE1-454a-993D-1307334C7A18}
DEFINE_GUID(CLSID_UsgfwStreamParserRFB, 
			0x820faa24, 0xbbe1, 0x454a, 0x99, 0x3d, 0x13, 0x7, 0x33, 0x4c, 0x7a, 0x18);


// {2F5042A0-45B2-493c-919D-E7043DC34843}
DEFINE_GUID(CLSID_UsgfwStreamParser, 
			0x2f5042a0, 0x45b2, 0x493c, 0x91, 0x9d, 0xe7, 0x4, 0x3d, 0xc3, 0x48, 0x43);


// {617348B2-1B25-4917-B161-02724C68E67A}
DEFINE_GUID(IID_IUsgfwTissueMotionDetector,
0x617348b2, 0x1b25, 0x4917, 0xb1, 0x61, 0x2, 0x72, 0x4c, 0x68, 0xe6, 0x7a);
DECLARE_INTERFACE_(IUsgfwTissueMotionDetector, IUnknown)
{
	STDMETHOD(SetNoiseLevel)(THIS_
		LONG nNoiseLevel
		) PURE;
	STDMETHOD_(LONG, GetNoiseLevel)(THIS_
		void) PURE;
	STDMETHOD(SetTolerance)(THIS_
		LONG nTolerance
		) PURE;
	STDMETHOD_(LONG, GetTolerance)(THIS_
		void) PURE;
	STDMETHOD(SetDetectionTimeout)(THIS_
		LONG nTimeout
		) PURE;
	STDMETHOD_(LONG, GetDetectionTimeout)(THIS_
		void) PURE;
	STDMETHOD(GetCurrentFormat) (THIS_
		AM_MEDIA_TYPE **ppMediaType
		) PURE;
	STDMETHOD_(BOOL, IsDetectionSupported)(THIS_
		const AM_MEDIA_TYPE *pMediaType
		) PURE;
	STDMETHOD_(BOOL, GetDetectionResult)(THIS_
		void) PURE;
	STDMETHOD(SetNotificationEventCode)(THIS_
		LONG nEventCode
		) PURE;
	STDMETHOD_(LONG, GetNotificationEventCode)(THIS_
		void) PURE;
};

// {7B6D6877-759C-4698-9544-BABBF2936307}
DEFINE_GUID(CLSID_UsgfwTissueMotionDetector,
0x7b6d6877, 0x759c, 0x4698, 0x95, 0x44, 0xba, 0xbb, 0xf2, 0x93, 0x63, 0x7);


#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */


#endif	//_USGFW_H_