///////////////////////////////
// View.cpp

#include "stdafx.h"
#include "View.h"
#include "resource.h"
#include "RibbonUI.h"

// Definitions for the CView class

int CView::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate is called automatically during window creation when a
    // WM_CREATE message received.

    // Tasks such as setting the icon, creating child windows, or anything
    // associated with creating windows are normally performed here.

    UNREFERENCED_PARAMETER(cs);

    TRACE("OnCreate\n");

    // Set the window's icon
    SetIconSmall(IDI_SMALL);
    SetIconLarge(IDI_SIMPLERIBBON);

    // Set the window title
    SetWindowText(LoadString(IDS_APP_TITLE).c_str());

    if (GetWinVersion() >= 2601)        // Ribbon only supported on Windows 7 and above
    {
        if (SUCCEEDED(CreateRibbon(*this)))
            TRACE("Ribbon Created Succesfully\n");
        else
            TRACE("Failed to create ribbon\n");
    }

    return 0;
}

void CView::OnDestroy()
{
    if (GetWinVersion() >= 2601)        // Ribbon only supported on Windows 7 and above
    {
        DestroyRibbon();
    }

    // End the application when the window is destroyed
    ::PostQuitMessage(0);
}

void CView::OnDraw(CDC& dc)
{
    // OnPaint is called automatically whenever a part of the
    // window needs to be repainted.

    // Centre some text in our view window
    CRect r = GetClientRect();
    r.top += GetRibbonHeight();
    dc.DrawText(_T("Simple Ribon Demo"), -1, r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::OnInitialUpdate()
{
    // OnInitialUpdate is called after the window is created.
    // Tasks which are to be done after the window is created go here.

    RedrawWindow();
    TRACE("OnInitialUpdate\n");
}

void CView::OnSize()
{
    CRect r = GetClientRect();
    r.top += GetRibbonHeight();
    InvalidateRect(r);
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // This function will be called automatically by Create. It provides an
    // opportunity to set various window parameters prior to window creation.
    // You are not required to set these parameters, any paramters which
    // aren't specified are set to reasonable defaults.

    // Set some optional parameters for the window
    cs.dwExStyle = WS_EX_CLIENTEDGE;        // Extended style
    cs.lpszClass = _T("View Window");       // Window Class
    cs.x = 50;                              // top x
    cs.y = 50;                              // top y
    cs.cx = 400;                            // width
    cs.cy = 300;                            // height
}

STDMETHODIMP CView::Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties)
{
    UNREFERENCED_PARAMETER(pCommandExecutionProperties);
    UNREFERENCED_PARAMETER(ppropvarValue);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(verb);

    HRESULT result = S_OK;
    switch(cmdID)
    {
    case cmdButton1:
        TRACE("Button 1\n");
        break;
    case cmdButton2:
        TRACE("Button 2\n");
        break;
    case cmdButton3:
        TRACE("Button 3\n");
        break;
    case cmdButton4:
        TRACE("Button 4\n");
        break;
    case cmdButton5:
        TRACE("Button 5\n");
        break;
    case cmdButton6:
        TRACE("Button 6\n");
        break;
    case cmdToggleButton1:
        TRACE("Toggle Button 1\n");
        break;
    case cmdToggleButton2:
        TRACE("Toggle Button 2\n");
        break;
    case IDC_CMD_EXIT:
        TRACE("Exit button\n");
        Close();
        break;
    default:
        TRACE("Unknown button\n");
        break;
    }

    return result;
}

STDMETHODIMP CView::OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32 reasonCode)
{
    UNREFERENCED_PARAMETER(viewId);
    UNREFERENCED_PARAMETER(pView);
    UNREFERENCED_PARAMETER(reasonCode);

    HRESULT result = E_NOTIMPL;

    // Checks to see if the view that was changed was a Ribbon view.
    if (UI_VIEWTYPE_RIBBON == typeId)
    {
        switch (verb)
        {
        case UI_VIEWVERB_CREATE:    // The view was newly created.
            result = S_OK;
            break;
        case UI_VIEWVERB_SIZE:      //  The ribbon's size has changed
            result = S_OK;
            break;
        case UI_VIEWVERB_DESTROY:   // The view was destroyed.
            result = S_OK;
            break;
        }
    }

    return result;
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:
        OnSize();
        break;  // and also do default processing for this message
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

