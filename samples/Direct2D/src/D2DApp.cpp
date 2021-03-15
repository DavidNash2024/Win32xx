/////////////////////////////
// D2DApp.cpp
//

#include "stdafx.h"
#include "D2DApp.h"


///////////////////////////////
// CD2DApp function definitions
//

// Constructor.
CD2DApp::CD2DApp() : m_pDirect2dFactory(NULL)
{
}

// Destructor.
CD2DApp::~CD2DApp()
{
    SafeRelease(&m_pDirect2dFactory);
    CoUninitialize();
}

// Create a Direct2D factory.
HRESULT CD2DApp::CreateDeviceIndependentResources()
{
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
}

// Called when the application starts.
BOOL CD2DApp::InitInstance()
{
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr))
    {
        // Initialize device-independent resources, such as the Direct2D factory.
        hr = CreateDeviceIndependentResources();

        if (SUCCEEDED(hr))
        {
            m_view.Create();    // Throws a CWinException on failure.
            m_view.SetWindowText(L"Direct2D Demo");
            return TRUE;
        }
        else
            ::MessageBox(0, L"Failed to start DirectX", L"Error", MB_OK);
    }
    else
        ::MessageBox(0, L"Failed to initialize COM", L"Error", MB_OK);

    return FALSE;
}

