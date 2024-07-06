/////////////////////////////
// Dockers.cpp
//

#include "stdafx.h"
#include "Dockers.h"


////////////////////////////////////
//  CDockSimple function definitions
//

// Constructor.
CDockSimple::CDockSimple()
{
    // Set the view window for the simple docker.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(8);

    SetCaption (_T("Simple View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockSimple::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}

// Handle the window's messages.
LRESULT CDockSimple::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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

/////////////////////////////////
// CDockText function definitions
//

// Constructor.
CDockText::CDockText()
{
    // Set the view window to our edit control.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(8);

    SetCaption(_T("Text View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockText::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}

// Handle the window's messages.
LRESULT CDockText::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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

////////////////////////////////////
// CDockClasses function definitions
//

// Constructor.
CDockClasses::CDockClasses()
{
    // Set the view window to our TreeView control.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(8);

    SetCaption(_T("Class View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockClasses::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
       cs.dwExStyle |= WS_EX_COMPOSITED;
}

// Handle the window's messages.
LRESULT CDockClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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


///////////////////////////////////
//  CDockFiles function definitions
//

// Constructor.
CDockFiles::CDockFiles()
{
    // Set the view window to our ListView control.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(8);

    SetCaption(_T("Files View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockFiles::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}

// Handle the window's messages.
LRESULT CDockFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
