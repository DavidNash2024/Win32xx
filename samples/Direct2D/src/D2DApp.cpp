/////////////////////////////
// D2DApp.cpp
//

#include "stdafx.h"
#include "D2DApp.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    CD2DApp app;
    app.Run();

    return 0;
}

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
            m_view.SetWindowText(_T("Direct2D Demo"));
            return TRUE;
        }
        else
            ::MessageBox(0, _T("Failed to start DirectX"), _T("Error"), MB_OK);
    }
    else
        ::MessageBox(0, _T("Failed to initialize COM"), _T("Error"), MB_OK);

    return FALSE;
}

