////////////////////////////////////////
// MainView.cpp
//  Definitions for the CMainView class.

#include "stdafx.h"
#include "MainView.h"
#include "resource.h"


int CMainView::OnCreate(CREATESTRUCT&)
{
    m_imageView.Create(*this);

    // Support Drag and Drop on this window
    DragAcceptFiles(TRUE);

    return 0;
}

// Called when part of the window needs to be redrawn.
void CMainView::OnDraw(CDC& dc)
{
    if (!m_imageView.GetImage().GetHandle())
    {
        // There is no image, so display a hint to get one
        // Use the message font for Windows 7 and higher.
        if (GetWinVersion() >= 2601)
        {
            NONCLIENTMETRICS info = GetNonClientMetrics();
            LOGFONT lf = DpiScaleLogfont(info.lfMessageFont, 10);
            dc.CreateFontIndirect(lf);
        }

        dc.SetBkColor(RGB(0, 0, 0));
        dc.SetTextColor(RGB(255, 255, 255));

        CRect rc = GetClientRect();

        dc.DrawText(L"Use the Menu or ToolBar to open a Bitmap File",
            -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

// Called when a file is dropped on the window.
// Loads the dropped file.
LRESULT CMainView::OnDropFiles(UINT, WPARAM wparam, LPARAM)
{
    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, 0, 0);

    if (length > 0)
    {
        CString fileName;
        DragQueryFile(hDrop, 0, fileName.GetBuffer(length), length + 1);
        fileName.ReleaseBuffer();
        DragFinish(hDrop);

        if (!m_imageView.LoadFileImage(fileName))
        {
            TRACE("Failed to load "); TRACE(fileName); TRACE("\n");
            m_imageView.SetScrollSizes(CSize(0, 0));
            RecalcLayout();
        }
    }

    return 0;
}

LRESULT CMainView::OnSize(UINT, WPARAM, LPARAM)
{
    RecalcLayout();
    return 0;
}

void CMainView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    // Set the extended style.
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CMainView::PreRegisterClass(WNDCLASS& wc)
{
    // Set the Window Class name.
    wc.lpszClassName = L"Main View";

    wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
}

void CMainView::RecalcLayout()
{
    if (m_imageView.GetImage().GetHandle())
    {
        CRect clientRect = GetClientRect();
        int imageWidth = GetImage().GetSize().cx;
        int imageHeight = GetImage().GetSize().cy;

        clientRect.left = std::max((clientRect.Width() - imageWidth) / 2, 0);
        clientRect.top = std::max((clientRect.Height() - imageHeight) / 2, 0);

        m_imageView.SetWindowPos(HWND_TOP, clientRect, SWP_SHOWWINDOW);
    }
    else
        m_imageView.SetWindowPos(HWND_TOP, CRect(0,0,0,0), 0);

    Invalidate();
}

// Process the main view's window messages.
LRESULT CMainView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DROPFILES:   return OnDropFiles(msg, wparam, lparam);
        case WM_SIZE:        return OnSize(msg, wparam, lparam);
        }

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

