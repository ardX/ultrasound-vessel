#pragma once


#include <SetupAPI.h>
extern "C" {
	// declare the WDMDDL C libraries used
//#include "hidsdi.h"

}


#include <Wbemidl.h>

class CUsgfw2;

class CUsgfwWindow
{


	class CWmiEventSink : public IWbemObjectSink
	{
	protected:
		LONG m_lRef;
		bool bDone;
		CUsgfwWindow* m_pUsgfwWindow;
	public:

		CWmiEventSink(CUsgfwWindow* pUsgfwWindow) :m_lRef(0), bDone(false), m_pUsgfwWindow(pUsgfwWindow) { }
		~CWmiEventSink() { bDone = true; }

		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

		virtual HRESULT STDMETHODCALLTYPE Indicate(
			LONG lObjectCount,
			IWbemClassObject __RPC_FAR* __RPC_FAR* apObjArray
		);

		virtual HRESULT STDMETHODCALLTYPE SetStatus(
			/* [in] */ LONG lFlags,
			/* [in] */ HRESULT hResult,
			/* [in] */ BSTR strParam,
			/* [in] */ IWbemClassObject __RPC_FAR* pObjParam
		);
	};

	//void WmiInitialize(void);
	//void WmiCleanup(void);
//	CComPtr<IWbemLocator> m_iWbemLocator;
//	CComPtr<IWbemServices> m_iWbemServices;
//	CComPtr<IUnsecuredApartment> m_iUnsecuredApartment;
//	CComPtr<IWbemObjectSink> m_wmiEventSink;
//	CComPtr<IUnknown> m_iStubUnk;
//	CComQIPtr<IWbemObjectSink> m_iWbemObjectSink;

//	CComPtr<IWbemServices> m_iWbemServicesWmi;

	bool m_fUsgScannerStateEvent;
	static DWORD WINAPI PnpDeviceCreateThread(LPVOID pParam);
	static DWORD WINAPI PnpDeviceDeleteThread(LPVOID pParam);
	static DWORD WINAPI TelemedUsgScannerStateEventThread(LPVOID pParam);

	// structures that will work as a parameters for thread functions.
	typedef struct
	{
		CUsgfw2 *pUsgfw2;
		BSTR strInstanceID;
		BSTR strDeviceID;
		UINT32 uScannerState1;
		UINT32 uScannerState2;
		UINT32 uSensorsState;
		UINT32 uLastErrorCode;
	} PNP_DEVICE_CREATE_PARAMS;

	typedef struct
	{
		CUsgfw2* pUsgfw2;
		BSTR strInstanceID;
		BSTR strDeviceID;
	} PNP_DEVICE_DELETE_PARAMS;

	typedef struct
	{
		CUsgfw2* pUsgfw2;
		BSTR strInstanceID;
		UINT32 uScannerState1;
		UINT32 uScannerState2;
		UINT32 uSensorsState;
		UINT32 uLastErrorCode;
	} PNP_SCANNER_STATE_CHANGE_PARAMS;
public:
	CUsgfwWindow(); // CUsgfw2* pUsgfw2
	virtual ~CUsgfwWindow(void);
	HWND GetHwnd(void);
	HWND Create(LPCWSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hwndParent, HMENU hmenu, HINSTANCE hinstance);
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL RegisterClass(HINSTANCE hInstance);

    


//	void OnPnpDeviceCreate(CString& strInstanceName, CString& strDeviceID);
//	void OnPnpDeviceDelete(CString& strInstanceName, CString& strDeviceID);
//	void OnTelemedUsgScannerStateEvent(CString& strWmiInstanceName, UINT32 uScannerState1, UINT32 uScannerState2, UINT32 uSensorsState, UINT32 uLastErrorCode);

protected:
	static const wchar_t m_szClassName[];
	HINSTANCE m_hInstance;
	HWND m_hwndParent;
	HWND m_hwnd;
	DWORD m_dwUserData;
	CUsgfw2 *m_pUsgfw2;
	PVOID m_hProbeNotify;
	PVOID m_hBeamformerNotify;
	PVOID m_hHidNotify;

	static LRESULT __stdcall WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//static DWORD WINAPI BeamformerArriveThread(LPVOID pParam);
	//static DWORD WINAPI BeamformerRemoveThread(LPVOID pParam);
	//static DWORD WINAPI ProbeArriveThread(LPVOID pParam);
	//static DWORD WINAPI ProbeRemoveThread(LPVOID pParam);

	HANDLE m_hWndThread;
public:
	static DWORD WINAPI StartWindow(LPVOID pParam);
	void RunWindow(void);

	//LRESULT OnDeviceChange(WPARAM nEventType, LPARAM dwData);
	//LRESULT OnPowerStateChange(WPARAM nEventType, LPARAM dwData);


	//void UpdateHidDevices(bool fEnable);
	void Close(void);
protected:
	void OnCreate(void);
	void PreDestroy(void);

	typedef struct tagHidEntry
	{
		DWORD dwVid;
		DWORD dwPid;
		DWORD dwUsagePage;
		DWORD dwUsage;
		DWORD dwPassword;
	} HID_ENTRY;

	static HID_ENTRY sm_HidSupported[];
	static int g_nInstance;
	void HidDeviceInitialize(PCHAR pDevicePath, bool fEnable);
	// void WriteReportToHidDevice(PCHAR pDevicePath, PBYTE pReportBuffer, int nReportLen);
};
