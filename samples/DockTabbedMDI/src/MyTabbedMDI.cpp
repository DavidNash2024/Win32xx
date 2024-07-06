/////////////////////////////
// MyTabbedMDI.cpp
//

#include "stdafx.h"
#include "MyTabbedMDI.h"
#include "Browser.h"
#include "Classes.h"
#include "Files.h"
#include "Output.h"
#include "Rect.h"
#include "Text.h"


////////////////////////////////////
// CMyTabbedMDI function definitions
//

// Constructor.
CMyTabbedMDI::CMyTabbedMDI()
{
}

// Adds a new MDI. The mdiChild parameter specifies the MDI type.
CWnd* CMyTabbedMDI::NewMDIChildFromID(int mdiChild)
{
    CWnd* pView = NULL;
    switch(mdiChild)
    {
    case ID_MDI_CLASSES:
        pView = new CViewClasses;
        break;
    case ID_MDI_FILES:
        pView = new CViewFiles;
        break;
    case ID_MDI_OUTPUT:
        pView = new CViewOutput;
        break;
    case ID_MDI_TEXT:
        pView = new CViewText;
        break;
    case ID_MDI_RECT:
        pView = new CViewRect;
        break;
    case ID_MDI_WEB:
        pView = new CViewWeb;
        break;
    default:
        TRACE("Unknown TabbedMDI id\n");
        break;
    }

    return pView;
}

// Called when the window handle (HWND) is attached to this object.
void CMyTabbedMDI::OnAttach()
{
    CTabbedMDI::OnAttach();
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        SetExStyle(WS_EX_COMPOSITED);
}

// Called after the window is moved or resized.
LRESULT CMyTabbedMDI::OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LockWindowUpdate();
    CTabbedMDI::OnWindowPosChanged(msg, wparam, lparam);
    UnlockWindowUpdate();
    UpdateWindow();
    return 0;
}

// Handle the window's messages.
LRESULT CMyTabbedMDI::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
