// ycapture.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "StdAfx.h"
#include <initguid.h>
#include "CaptureSource.h"

// 関数宣言
#define CreateComObject(clsid, iid, var) CoCreateInstance( clsid, NULL, CLSCTX_INPROC_SERVER, iid, (void **)&var);

STDAPI AMovieSetupRegisterServer( CLSID   clsServer, LPCWSTR szDescription, LPCWSTR szFileName, LPCWSTR szThreadingModel = L"Both", LPCWSTR szServerType     = L"InprocServer32" );
STDAPI AMovieSetupUnregisterServer( CLSID clsServer );

// フィルタ定義
CFactoryTemplate g_Templates[] = 
{
    { NAME_CAPTURESOURCE, &CLSID_CaptureSource, CaptureSource::CreateInstance, NULL, &sudCaptureSource }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


// 関数定義

// フィルタを登録します。
// 参考：
// Stack Overflow - "Fake" DirectShow video capture device
// http://stackoverflow.com/questions/1376734/fake-directshow-video-capture-device
STDAPI RegisterFilters(BOOL bRegister)
{
    HRESULT hr = NOERROR;
    WCHAR achFileName[MAX_PATH];
    char achTemp[MAX_PATH];
    ASSERT(g_hInst != 0);

    if( 0 == GetModuleFileNameA(g_hInst, achTemp, sizeof(achTemp))) 
        return AmHresultFromWin32(GetLastError());

    MultiByteToWideChar(CP_ACP, 0L, achTemp, lstrlenA(achTemp) + 1, 
                       achFileName, NUMELMS(achFileName));
  
    hr = CoInitialize(0);
    if(bRegister)
    {
        hr = AMovieSetupRegisterServer(CLSID_CaptureSource, NAME_CAPTURESOURCE, achFileName, L"Both", L"InprocServer32");
    }

    if(SUCCEEDED(hr)) {
        IFilterMapper2 *fm = NULL;
        hr = CreateComObject(CLSID_FilterMapper2, IID_IFilterMapper2, fm);

        if(SUCCEEDED(hr)) {
            if(bRegister) {
                IMoniker *pMoniker = 0;
                REGFILTER2 rf2;
                rf2.dwVersion = 1;
                rf2.dwMerit = MERIT_DO_NOT_USE;
                rf2.cPins = 1;
                rf2.rgPins = &sudCaptureSourceOut;
                hr = fm->RegisterFilter(CLSID_CaptureSource, NAME_CAPTURESOURCE, &pMoniker, &CLSID_VideoInputDeviceCategory, NULL, &rf2);
            }else{
                hr = fm->UnregisterFilter(&CLSID_VideoInputDeviceCategory, 0, CLSID_CaptureSource);
            }
        }
		if(fm) {
			fm->Release();
			fm = NULL;
		}
    }

	if(SUCCEEDED(hr) && !bRegister) {
		hr = AMovieSetupUnregisterServer(CLSID_CaptureSource);
	}

    CoFreeUnusedLibraries();
    CoUninitialize();
    return hr;
}

// 登録します。
STDAPI DllRegisterServer()
{
    return RegisterFilters(TRUE);
}

// 登録を解除します。
STDAPI DllUnregisterServer()
{
    return RegisterFilters(FALSE);
}
