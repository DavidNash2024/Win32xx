/////////////////////////////
// D2DApp.cpp
//

#include "pch.h"
#include "D2DApp.h"

///////////////////////////////
// CD2DApp function definitions
//

// Called when the application starts.
BOOL CD2DApp::InitInstance()
{
    try
    {
        m_view.Create(); // Drops straight into CD2DView::OnCreate
        m_view.SetWindowText(L"Direct2D Demo");
        return TRUE;
    }
    catch (const CWinException& e)
    {
        ::MessageBox(nullptr, e.GetText(), L"Window Creation Error", MB_OK | MB_ICONERROR);
    }

    return FALSE;
}