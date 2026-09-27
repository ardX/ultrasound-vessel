#pragma once
#include "stdafx.h"
#include <initguid.h>

class SampleGrabberCallback : public ISampleGrabberCB
{
public:
	SampleGrabberCallback();
	~SampleGrabberCallback();
	//void SetUsgWnd(CsampleDlg* wnd, int x, int y);
	void SetPlugin(ISampleGrabber* plugin);

private:
	ISampleGrabber* m_plugin;
	//CsampleDlg* m_usg_wnd;
	int m_usg_x;
	int m_usg_y;

	AM_MEDIA_TYPE *m_pMediaType;
public:

	// begin: IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppObj);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	long m_nRefCount;
	// end: IUnknown

	// begin: ISampleGrabberCB
	STDMETHODIMP BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen);
	STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample);

	// end: ISampleGrabberCB

}; // class SampleGrabberCallback

ISampleGrabberFilter* sample_grabber_filter;	// sample grabber control for desired stream (B mode RF) from that we get plugin interface
ISampleGrabber* m_sample_grabber_plugin;		// plugin to that we set callback
SampleGrabberCallback* m_sample_grabber_callback;	// sample grabber calback where we receive RF data
LONG m_nLineIndex;		// the index of RF line inside RF window
int m_nSubFrameIndex;
IUsgWindowRF* m_pUsgWindowRF;					// RF (radio frequency) data window
IUsgDataSourceRF* m_pUsgDataSourceRF;			// interface to set desired RF (radio frequency) data type
volatile bool using_rf_stream = true;

// SampleCB(double SampleTime, IMediaSample *pSample);
void ProcessSample(PBYTE pBuffer, int nBufferLen, PULTRASOUNDRF_INFO pFrameInfo, int nSubFrameIndex, bool updateGUI);
