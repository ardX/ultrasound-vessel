//#include "pch.h"

#include "stdafx.h"
#include ".\usgfwwindow.h"
//#include "Usgfw2imp.h"
#include <dbt.h>

const wchar_t CUsgfwWindow::m_szClassName[] = L"Usgfw2Imagingwindow";
// list of supported HID devices
CUsgfwWindow::HID_ENTRY CUsgfwWindow::sm_HidSupported[] = 
{
	// Metola Keyboard
	{0xffff, 0x1035, 0x40, 0x01, 200},
	// Sono Keyboard
	{0xffff, 0x1036, 0x40, 0x01, 157},
	// Basda Keyboard
	{0xffff, 0x1037, 0x40, 0x01, 157},
	// Keyboard LB2
	{0xffff, 0x1038, 0x40, 0x01, 189},
	// Keyboard Minston
	{0xffff, 0x1039, 0x40, 0x01, 193},
	// Keyboard eCare
	{0xffff, 0x103a, 0x40, 0x01, 204},
	// Keyboard Sonoray
	{0xffff, 0x103b, 0x40, 0x01, 185},
};

int CUsgfwWindow::g_nInstance = 0;

CUsgfwWindow::CUsgfwWindow() // CUsgfw2* pUsgfw2
: m_hInstance(0)
, m_hwndParent(NULL)
, m_hwnd(NULL)
, m_dwUserData(0)
//, m_pUsgfw2(pUsgfw2)
, m_hProbeNotify(NULL)
, m_hBeamformerNotify(NULL)
, m_hHidNotify(NULL)
, m_hWndThread(NULL)
{
	g_nInstance ++;
}

CUsgfwWindow::~CUsgfwWindow(void)
{
	do
	{
		g_nInstance --;
		if(!m_hwnd)
			break;
		//SetWindowLong(m_hwnd,GWL_USERDATA,0);
		PreDestroy();
		::DestroyWindow(m_hwnd);
		/*
		if(!m_hWndThread)
			::CloseWindow(m_hwnd);
		else
			PostThreadMessage((DWORD)m_hWndThread, WM_CLOSE, 0 ,0);
		*/
		m_hwnd = NULL;

	} while(false);

}


void CUsgfwWindow::Close(void)
{
	PostMessage(m_hwnd, WM_CLOSE, 0, 0);
}


HWND CUsgfwWindow::GetHwnd(void)
{
	return m_hwnd;
}

HWND CUsgfwWindow::Create(LPCWSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hwndParent, HMENU hmenu, HINSTANCE hinstance)
{
	m_hInstance=hinstance;
	m_hwnd=CreateWindow(m_szClassName,lpszWindowName,dwStyle,x,y,nWidth,nHeight,hwndParent,hmenu,hinstance,this);
	m_hwndParent = hwndParent;
	return m_hwnd;
}






void CUsgfwWindow::RunWindow(void)
{
	DWORD dwThreadID = 0;
	m_hWndThread = ::CreateThread(NULL, 0, StartWindow, this, 0, &dwThreadID);
}

DWORD WINAPI CUsgfwWindow::StartWindow(LPVOID pParam)
{
	// CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CoInitialize(NULL);
	// create window
	CUsgfwWindow* pUsgfwWindow = (CUsgfwWindow*)pParam;
	HWND hWnd = pUsgfwWindow->Create(L"Usgfw2Monitor", 0, 0, 0, 512, 512, NULL, 0, 0);
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	// run message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CoUninitialize();

	return (int)msg.wParam;
}

LRESULT CUsgfwWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT retValue = 0;
	switch(uMsg) 
	{
	case WM_DEVICECHANGE:
//		retValue = OnDeviceChange(wParam, lParam);
		break;
	case WM_POWERBROADCAST:
//		retValue = OnPowerStateChange(wParam, lParam);
		break;
	case WM_CLOSE:
		// retValue = ::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		PreDestroy();
//		UpdateHidDevices(false);
		::DestroyWindow(m_hwnd);
		break;
	default:
		retValue = ::DefWindowProc(m_hwnd, uMsg, wParam, lParam);
		break;
	}
	return retValue;
}


LRESULT __stdcall CUsgfwWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CUsgfwWindow* pUsgWindow=NULL;
	switch(uMsg)
	{
	case WM_CREATE:
//		CoInitialize(NULL);
		pUsgWindow=(CUsgfwWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)pUsgWindow);
		pUsgWindow->m_hwnd = hwnd;
		pUsgWindow->OnCreate();
		break;
	case WM_DESTROY:
		pUsgWindow = (CUsgfwWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
		SetWindowLongPtr(hwnd,GWLP_USERDATA,0);
		if(pUsgWindow)
		{
			delete pUsgWindow;
		}
		pUsgWindow = NULL;
		//::PostQuitMessage(0);
		// CoUninitialize();
		break;
	default:
		pUsgWindow=(CUsgfwWindow*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
		break;
	}
	if(pUsgWindow != NULL)
		return pUsgWindow->WndProc(uMsg,wParam,lParam);
	else
		return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

BOOL CUsgfwWindow::RegisterClass(HINSTANCE hInstance)
{
	ATOM aWndClass;
	WNDCLASS wc;

	ZeroMemory(&wc,sizeof(wc));
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)(CUsgfwWindow::WindowProc);
	wc.lpszClassName = (LPCWSTR)m_szClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	aWndClass = ::RegisterClass(&wc);

	return (aWndClass != 0);
}



// function is called after window is created
void CUsgfwWindow::OnCreate(void)
{
	// register for device change notifications
	DEV_BROADCAST_DEVICEINTERFACE filterData;
	ZeroMemory(&filterData, sizeof(DEV_BROADCAST_DEVICEINTERFACE));   
	filterData.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	filterData.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
//	filterData.dbcc_classguid = CLSID_UltrasonicTransducer; // AM_KSCATEGORY_CAPTURE;
	m_hProbeNotify = ::RegisterDeviceNotification(m_hwnd, &filterData, DEVICE_NOTIFY_WINDOW_HANDLE);        

	ZeroMemory(&filterData, sizeof(DEV_BROADCAST_DEVICEINTERFACE));   
	filterData.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	filterData.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
//	filterData.dbcc_classguid = CLSID_UltrasonicBeamformer; // AM_KSCATEGORY_CAPTURE;
	m_hBeamformerNotify = ::RegisterDeviceNotification(m_hwnd, &filterData, DEVICE_NOTIFY_WINDOW_HANDLE);

	ZeroMemory(&filterData, sizeof(DEV_BROADCAST_DEVICEINTERFACE));   
	filterData.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	filterData.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	// get HID GUID
//	::HidD_GetHidGuid(&filterData.dbcc_classguid);
	m_hHidNotify = ::RegisterDeviceNotification(m_hwnd, &filterData, DEVICE_NOTIFY_WINDOW_HANDLE);

	//	DWORD dwThreadID;
//	::CreateThread(NULL, 0, BeamformerArriveThread, this, 0, &dwThreadID);
//	if(m_pUsgfw2)
//		m_pUsgfw2->OnBeamformerArrive();
//	WmiInitialize();
}

// function is called before window will be destroy
void CUsgfwWindow::PreDestroy(void)
{
//	WmiCleanup();
	m_pUsgfw2 = NULL;
	// unregister from device change notifications
	if(m_hProbeNotify)
		::UnregisterDeviceNotification(m_hProbeNotify);
	if(m_hBeamformerNotify)
		::UnregisterDeviceNotification(m_hBeamformerNotify);
	if(m_hHidNotify)
		::UnregisterDeviceNotification(m_hHidNotify);

	m_hProbeNotify = NULL;
	m_hBeamformerNotify = NULL;
	m_hHidNotify = NULL;
}




