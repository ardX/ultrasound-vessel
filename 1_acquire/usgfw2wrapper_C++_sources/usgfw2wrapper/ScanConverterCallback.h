#pragma once
#include "stdafx.h"
#include <initguid.h>



DEFINE_GUID(IID_IUsgfwScanConverterPlugin, 
0x5b5de697, 0x183c, 0x4b76, 0x9d, 0xfb, 0x4d, 0xb0, 0x58, 0xc7, 0x3e, 0x75);

// {AD8F939C-689E-4efa-B090-DC944274C7AC}
DEFINE_GUID(IID_IUsgfwScanConverterPluginCB, 
0xad8f939c, 0x689e, 0x4efa, 0xb0, 0x90, 0xdc, 0x94, 0x42, 0x74, 0xc7, 0xac);


typedef enum tagUSCP_METHODTOCALL
{
	USPC_SAMPLE_INPUT_OUTPUT = 0x01,
	USPC_BUFFER_INPUT_OUTPUT = 0x02,
	USPC_SAMPLE_INPUT_INTERIM = 0x04,
	USPC_BUFFER_INPUT_INTERIM = 0x08,
	USPC_SAMPLE_INTERIM_OUTPUT = 0x10,
	USPC_BUFFER_INTERIM_OUTPUT = 0x20,
} USPC_METHODTOCALL;

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



class ScanConverterCallback: public IUsgfwScanConverterPluginCB
{
public:
	ScanConverterCallback();
	~ScanConverterCallback();
	void SetUsgWnd(HWND wnd, int x, int y);
	void SetPlugin(IUsgfwScanConverterPlugin* plugin);

public: //private:

	IUsgfwScanConverterPlugin* m_plugin;
	HWND m_usg_wnd;
	int m_usg_x;
    int m_usg_y;

public:

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppObj);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	long m_nRefCount;

	// begin: IUsgfwScanConverterPluginCB



	// receives pointers to input and output media samples
			// receives pointers to input and output media samples
	STDMETHOD(SampleCB) (THIS_
		IMediaSample *pSampleIn,
		IMediaSample *pSampleOut,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) {
		return S_OK;
	}

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
		);

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
		) {
		return S_OK;
	}

	// receves pointers to input media sample and intermediatesample buffer
	STDMETHOD(InInterimSampleCB) (THIS_
		IMediaSample *pSampleIn,
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) {
		return S_OK;
	}

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
		) {
		return S_OK;
	}

	// receives pointers to output media sample and intermediate sample buffer
	STDMETHOD(InterimOutSampleCB) (THIS_
		PBYTE pBufferInterim,
		int nInterimBufferLen,
		IMediaSample *pSampleIn,
		int nOutX1,
		int nOutY1,
		int nOutX2,
		int nOutY2
		) {
		return S_OK;
	}

	// receives conversion parameter change pin index
	// if parameter is negative parameter was changed by some filter interface
	STDMETHOD(ParameterCB) (THIS_
		int nPin
		) {
		return S_OK;
	}



};

ScanConverterCallback* m_scan_converter_callback;
IUsgScanConverterPlugin* scp;
AM_MEDIA_TYPE *m_pMediaType;