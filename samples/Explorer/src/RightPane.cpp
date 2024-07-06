//////////////////////////
// RightPane.cpp
//

#include "stdafx.h"
#include "LeftPane.h"
#include "RightPane.h"

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
CDocker* CRightPane::NewDockerFromID(int id)
{
    CDocker* pDock = NULL;
    switch (id)
    {
    case ID_DOCK_LEFTPANE:
        pDock = new CLeftPane;
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return pDock;
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
