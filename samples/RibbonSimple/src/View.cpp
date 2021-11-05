/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "Resource.h"
#include "RibbonUI.h"

//////////////////////////////
// CView function definitions.
//
int CView::OnCreate(CREATESTRUCT&)
{
    // OnCreate is called automatically during window creation when a
    // WM_CREATE message received.

    // Tasks such as setting the icon, creating child windows, or anything
    // associated with creating windows are normally performed here.

    TRACE(L"OnCreate\n");

    // Set the window's icon
    SetIconSmall(IDI_SMALL);
    SetIconLarge(IDI_SIMPLERIBBON);

    // Set the window title
    SetWindowText(LoadString(IDS_APP_TITLE).c_str());

    if (GetWinVersion() >= 2601)        // Ribbon only supported on Windows 7 and above
    {
        if (SUCCEEDED(CreateRibbon(*this)))
            TRACE(L"Ribbon Created Successfully\n");
        else
            TRACE(L"Failed to create ribbon\n");
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
    dc.DrawText(L"Simple Ribbon Demo", -1, r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::OnInitialUpdate()
{
    // OnInitialUpdate is called after the window is created.
    // Tasks which are to be done after the window is created go here.

    RedrawWindow();
    TRACE(L"OnInitialUpdate\n");
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
    cs.lpszClass = L"View Window";       // Window Class
    cs.x = 50;                              // top x
    cs.y = 50;                              // top y
    cs.cx = 400;                            // width
    cs.cy = 300;                            // height
}

STDMETHODIMP CView::Execute(UINT32 cmdID, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*)
{
    HRESULT result = S_OK;

    switch(cmdID)
    {
    case cmdButton1:
        TRACE(L"Button 1\n");
        break;
    case cmdButton2:
        TRACE(L"Button 2\n");
        break;
    case cmdButton3:
        TRACE(L"Button 3\n");
        break;
    case cmdButton4:
        TRACE(L"Button 4\n");
        break;
    case cmdButton5:
        TRACE(L"Button 5\n");
        break;
    case cmdButton6:
        TRACE(L"Button 6\n");
        break;
    case cmdToggleButton1:
        TRACE(L"Toggle Button 1\n");
        break;
    case cmdToggleButton2:
        TRACE(L"Toggle Button 2\n");
        break;
    case IDC_CMD_EXIT:
        TRACE(L"Exit button\n");
        Close();
        break;
    default:
        TRACE(L"Unknown button\n");
        break;
    }

    return result;
}

STDMETHODIMP CView::OnViewChanged(UINT32, UI_VIEWTYPE typeId, IUnknown*, UI_VIEWVERB verb, INT32)
{
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
            Invalidate();
            result = S_OK;
            break;
        case UI_VIEWVERB_DESTROY:   // The view was destroyed.
            result = S_OK;
            break;
        case UI_VIEWVERB_ERROR:
            result = E_FAIL;
            break;
        }
    }

    return result;
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
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

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

