#pragma once
#include <ObjBase.h>

const WCHAR FName[]  = L"BVA COM";
const WCHAR VerInd[] = L"BVAC.Component";
const WCHAR ProgId[] = L"Addition.Component.7";

// {347CC716-94FA-412C-8B04-AAF0116CC8F0}



//GUID CLSID_CAddObj = {33F4C3B6-7153-4094-9E3D-CED37024302A};
// {12BF3B01-8DAC-4efd-8BC3-9D074F17061C}
//DEFINE_GUID(<<name>>, 
//0x12bf3b01, 0x8dac, 0x4efd, 0x8b, 0xc3, 0x9d, 0x7, 0x4f, 0x17, 0x6, 0x1c);

//GUID CLSID_CAddObj;


//#ifndef CLSID_CAddObj
//DEFINE_GUID(CLSID_CAddObj, 0x347cc716, 0x94fa, 0x412c, 0x8b, 0x4, 0xaa, 0xf0, 0x11, 0x6c, 0xc8, 0xf0);
//#endif




//#ifndef hAddObject
//#define hAddObject
//#endif

//#ifndef hAddObject
//#define HMODULE hAddObject = GetModuleHandle(NULL);
//#endif

//extern "C"  __declspec(dllexport) HRESULT DllInstall_new(char* s);

//

//extern "C"  __declspec(dllexport) HRESULT DllRegisterServer();
//extern "C"  __declspec(dllexport) HRESULT DllUnregisterServer();

//
STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();
STDAPI DllInstall_new(char* s);

// This function will register a component in the Registry.
// The component calls this function from its DllRegisterServer function.
HRESULT RegisterServer(HMODULE hModule, 
                       const CLSID& clsid, 
                       const WCHAR* szFriendlyName,
                       const WCHAR* szVerIndProgID,
                       const WCHAR* szProgID) ;

// This function will unregister a component.  Components
// call this function from their DllUnregisterServer function.
HRESULT UnregisterServer(const CLSID& clsid,
                         const WCHAR* szVerIndProgID,
                         const WCHAR*  szProgID) ;