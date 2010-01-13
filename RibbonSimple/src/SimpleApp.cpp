////////////////////////////////////////
// SimpleApp.cpp

#include "SimpleApp.h"


// Definitions for the CSimpleApp class
CSimpleApp::CSimpleApp()
{
    // Constructor for CSimpleApp
}

CSimpleApp::~CSimpleApp()
{ 
	CoUninitialize();
}

BOOL CSimpleApp::InitInstance()
{
    // This function is called automatically when the application starts

	HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        return FALSE;
    }

    // Create the Window
    m_View.Create();

	return TRUE;
}
