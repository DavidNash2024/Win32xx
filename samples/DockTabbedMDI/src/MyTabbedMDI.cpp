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

using namespace std;

////////////////////////////////////
// CMyTabbedMDI function definitions
//

// Adds a new MDI. The mdiChild parameter specifies the MDI type.
WndPtr CMyTabbedMDI::NewMDIChildFromID(int mdiChild)
{
    WndPtr view;
    switch(mdiChild)
    {
    case ID_MDI_CLASSES:
        view = make_unique<CViewClasses>();
        break;
    case ID_MDI_FILES:
        view = make_unique<CViewFiles>();
        break;
    case ID_MDI_OUTPUT:
        view = make_unique<CViewOutput>();
        break;
    case ID_MDI_TEXT:
        view = make_unique<CViewText>();
        break;
    case ID_MDI_RECT:
        view = make_unique<CViewRect>();
        break;
    case ID_MDI_WEB:
        view = make_unique<CViewWeb>();
        break;
    default:
        TRACE("Unknown TabbedMDI id\n");
        break;
    }

    return view;
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
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
