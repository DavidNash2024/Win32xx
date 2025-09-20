/////////////////////////////
// TestWindow.cpp
//

#include "StdAfx.h"
#include "TestWindow.h"
#include "TestApp.h"
#include "resource.h"


///////////////////////////////////
// CTestWindow function definitions.
//

// OnCommand responds to menu and and toolbar input.
BOOL CTestWindow::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {;
    case IDM_FILE_EXIT:     return OnFileExit();
    case IDM_VIEW_DIALOG:   return OnViewDialog();
    }

    return FALSE;
}

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
// Tasks such as setting the icon, creating child windows, or anything
// associated with creating windows are normally performed here.
int CTestWindow::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Set the window title
    SetWindowText(LoadString(IDW_MAIN));

    TRACE("OnCreate\n");
    return 0;
}

// Called when the window is destroyed.
void CTestWindow::OnDestroy()
{
    // End the application when the window is destroyed.
    ::PostQuitMessage(0);
}

// OnPaint is called automatically whenever a part of the
// window needs to be repainted.
void CTestWindow::OnDraw(CDC& dc)
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = info.lfMessageFont;
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window.
    CRect rc = GetClientRect();
    CString cs("Create additional dialogs from the menu");
    dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

// Respond to the File Exit menu item.
BOOL CTestWindow::OnFileExit()
{
    // End the application
    Close();
    return TRUE;
}

// OnInitialUpdate is called after the window is created.
// Tasks which are to be done after the window is created go here.
void CTestWindow::OnInitialUpdate()
{
    TRACE("OnInitialUpdate\n");
}

// Called when the window is resized.
LRESULT CTestWindow::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Force the window to be repainted during resizing.
    Invalidate();

    return FinalWindowProc(msg, wparam, lparam);
}

// Respond to the Show Dialog menu item.
BOOL CTestWindow::OnViewDialog()
{
    CTestApp* pApp = static_cast<CTestApp*>(GetApp());
    pApp->ShowDllDialog();
    return TRUE;
}

// This function will be called automatically by Create. It provides an
// opportunity to set various window parameters prior to window creation.
// You are not required to set these parameters. Any parameters which
// aren't specified are set to reasonable defaults.
void CTestWindow::PreCreate(CREATESTRUCT& cs)
{
    // Set some optional parameters for the window.
    cs.x = DpiScaleInt(50);               // top x
    cs.y = DpiScaleInt(50);               // top y
    cs.cx = DpiScaleInt(400);             // width
    cs.cy = DpiScaleInt(300);             // height
    cs.hMenu = m_menu;                    // menu
}

// This function is our message procedure. We process the messages for
// the view window here.  Unprocessed messages are passed on for
// default processing.
LRESULT CTestWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_SIZE:           return OnSize(msg, wparam, lparam);
    }

    // Pass unhandled messages on for default processing.
    return WndProcDefault(msg, wparam, lparam);
}
