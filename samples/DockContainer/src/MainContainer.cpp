/////////////////////////////
// MainContainer.cpp
//

#include "stdafx.h"
#include "MainContainer.h"


///////////////////////////////////////
// CMainContainer function definitions
//


void CMainContainer::AddContainer(CDockContainer* pContainer, BOOL insert, BOOL selectPage)
{
    // Call the base class function first.
    CDockContainer::AddContainer(pContainer, insert, selectPage);

    // Use different dock styles when we have a single tab vs multiple tabs.
    DWORD dockStyle = GetDocker()->GetDockStyle();
    if (GetItemCount() > 1)
        dockStyle = dockStyle & ~(DS_NO_CLOSE | DS_NO_UNDOCK);
    else
        dockStyle = dockStyle | DS_NO_CLOSE | DS_NO_UNDOCK;

    GetDocker()->SetDockStyle(dockStyle);
}

void CMainContainer::RemoveContainer(CDockContainer* pContainer, BOOL updateParent)
{
    // Call the base class function first.
    CDockContainer::RemoveContainer(pContainer, updateParent);

    // Use different dock styles when we have a single tab vs multiple tabs.
    DWORD dockStyle = GetDocker()->GetDockStyle();
    if (GetItemCount() > 1)
        dockStyle = dockStyle & ~(DS_NO_CLOSE | DS_NO_UNDOCK);
    else
        dockStyle = dockStyle | DS_NO_CLOSE | DS_NO_UNDOCK;

    GetDocker()->SetDockStyle(dockStyle);
}

// Handle the window's messages.
LRESULT CMainContainer::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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