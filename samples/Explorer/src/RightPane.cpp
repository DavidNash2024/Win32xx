//////////////////////////
// RightPane.cpp
//

#include "stdafx.h"
#include "LeftPane.h"
#include "RightPane.h"

using namespace std;

//////////////////////////////////
// CRightPane function definitions
//

// Constructor.
CRightPane::CRightPane()
{
    SetView(m_view);
}

// Adds a new docker. The id specifies the dock type.
// The id is used by LoadDockRegistrySettings to restore the
// previous splitter window arrangement.
DockPtr CRightPane::NewDockerFromID(int id)
{
    DockPtr docker;
    switch (id)
    {
    case ID_DOCK_LEFTPANE:
        docker = make_unique<CLeftPane>();
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return docker;
}

// Handle the window's messages.
LRESULT CRightPane::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
