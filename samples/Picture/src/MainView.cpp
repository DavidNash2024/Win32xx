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

// Called when a file is dropped on the window.
LRESULT CMainView::OnDropFiles(UINT, WPARAM wparam, LPARAM)
{
    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, nullptr, 0);

    if (length > 0)
    {
        CString FileName;
        DragQueryFile(hDrop, 0, FileName.GetBuffer(length), length + 1);
        FileName.ReleaseBuffer();

        if (!m_imageView.LoadPictureFile(FileName))
            m_imageView.NewPictureFile();
    }

    DragFinish(hDrop);
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
    if (m_imageView.GetPicture())
    {
        CRect clientRect = GetClientRect();
        int imageWidth = m_imageView.GetImageSize().cx;
        int imageHeight = m_imageView.GetImageSize().cy;

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

