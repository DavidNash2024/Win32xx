
#include "stdafx.h"
#include "D2DApp.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		CD2DApp app;
		app.Run();
		
		CoUninitialize();
	}

	return 0;
}

CD2DApp::CD2DApp() : m_pDirect2dFactory(NULL)
{
}

CD2DApp::~CD2DApp()
{
    SafeRelease(&m_pDirect2dFactory);
}

HRESULT CD2DApp::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    return hr;
}

BOOL CD2DApp::InitInstance()
{
	HRESULT hr;

	// Initialize device-independent resources, such as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		m_View.Create();
		m_View.SetWindowText(_T("Direct2D Demo"));
	}

	return TRUE;
}

