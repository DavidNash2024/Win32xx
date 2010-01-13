///////////////////////////////
// Ribbon.cpp

#include "Ribbon.h"
#include "Application.h"


bool CRibbon::InitializeFramework(HWND hWnd)
{
    // Here we instantiate the Ribbon framework object.
    HRESULT hr = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pFramework));
    if (FAILED(hr))
    {
        return false;
    }   

    // Next, we create the application object (IUIApplication) and call the framework Initialize method, 
    // passing the application object and the host HWND that the Ribbon will attach itself to.
    hr = CApplication::CreateInstance(&m_pApplication);
    if (FAILED(hr))
    {
        return false;
    } 

    hr = m_pFramework->Initialize(hWnd, m_pApplication);
    if (FAILED(hr))
    {
        return false;
    }

    // Finally, we load the binary markup.  This will initiate callbacks to the IUIApplication object 
    // that was provided to the framework earlier, allowing command handlers to be bound to individual
    // commands.
    hr = m_pFramework->LoadUI(GetModuleHandle(NULL), L"SIMPLERIBBON_RIBBON");
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}


void CRibbon::DestroyFramework()
{
    if (m_pFramework)
    {
        m_pFramework->Destroy();
        m_pFramework->Release();
        m_pFramework = NULL;
    }

    if (m_pApplication)
    {
        m_pApplication->Release();
        m_pApplication = NULL;
    }
}

