/////////////////////////////////////////////////////////////
// DialogHolder.cpp - Definitions for the CDialogHolder class
//


#include "stdafx.h"
#include "DialogHolder.h"


void CDialogHolder::PreCreate(CREATESTRUCT& cs)
{
    // Remove the visible style
    cs.style &= ~WS_VISIBLE;
}

// Displays the dialog defined by the specified dialog template array
// in the bottom right corner of the frame. The dialog is contained within
// a dialog holder if it is a child window, otherwise it is a top level
// window. The size and position of the dialog (and dialog holder if any)
// is adjusted for the frame window's DPI.
void CDialogHolder::ShowDialog(CWnd* pFrame, unsigned char* dlgArray)
{
    assert(pFrame);
    assert(dlgArray);

    // Destroy the current dialog and dialog holder.
    m_dialog.Destroy();
    Destroy();

    // Create the dialog holder window, initially hidden.
    CreateEx(0, L"DialogHolder", L"", 0, CRect(), 0, 0);
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);
    SetWindowText(L"Dialog Holder");

    try
    {
        // Create the modeless dialog from the dialog template array.
        // A dialog with the WS_CHILD style will be a child window of
        // the dialog holder, otherwise it will a top level owned
        // window of the dialog holder.
        m_dialog.SetDialogTemplate((LPCDLGTEMPLATE)dlgArray);
        m_dialog.Create(*this);

        // Retrieve the frame window's DPI.
        HWND hFrameWnd = pFrame->GetHwnd();
        UINT targetDpi = ::GetDpiForWindow(hFrameWnd);

        // Fetch the target frame boundaries (native screen pixels)
        CRect frameRect = pFrame->GetWindowRect();

        DWORD style = m_dialog.GetStyle();
        if (style & WS_CHILD)
        {
            // The dialog is a child window of the dialog holder.
            // We position it by positioning the dialog holder.

            // Retrieve the dialog's client rect.
            CRect dlgClientRect = m_dialog.GetClientRect();

            // Scale the dialog's width and height to frame's DPI.
            int targetClientWidth = ::MulDiv(dlgClientRect.Width(), targetDpi, 96);
            int targetClientHeight = ::MulDiv(dlgClientRect.Height(), targetDpi, 96);

            // Calculates the required size of the dialog rectangle, based
            // on the desired size of the client rectangle and the provided DPI.
            RECT targetWindowRect = { 0, 0, targetClientWidth, targetClientHeight };
            DWORD exStyle = m_dialog.GetExStyle();
            BOOL hasMenu = (::GetMenu(m_dialog.GetHwnd()) != nullptr);
            ::AdjustWindowRectExForDpi(&targetWindowRect, style, hasMenu, exStyle, targetDpi);

            // Calculate the child dialog's width and height.
            int finalWidth = targetWindowRect.right - targetWindowRect.left;
            int finalHeight = targetWindowRect.bottom - targetWindowRect.top;

            // Calculate the dialog holder's position.
            int left = frameRect.right - finalWidth;
            int top = frameRect.bottom - finalHeight;

            // Calculate the dialog holder's padding values.
            CRect holderRect = GetWindowRect();
            CRect holderClientRect = GetClientRect();
            int holderFramePaddingY = holderRect.Height() - holderClientRect.Height();
            int holderFramePaddingX = holderRect.Width() - holderClientRect.Width();

            // Set the position of the dialog holder.
            SetWindowPos(HWND_TOPMOST, left, top - holderFramePaddingY,
                finalWidth + holderFramePaddingX, finalHeight + holderFramePaddingY,
                SWP_SHOWWINDOW);

            // Set the position of the dialog child window.
            m_dialog.SetWindowPos(HWND_TOP, 0, 0, finalWidth, finalHeight, SWP_NOACTIVATE | SWP_SHOWWINDOW);
        }
        else
        {
            // The dialog is a top level window, so we use a different technique to
            // position it.

            // Send an initial SetWindowPos to force the OS to snap the popup dialog
            // to the target monitor's DPI engine without making it visible yet.
            m_dialog.SetWindowPos(nullptr, frameRect.left, frameRect.top, 0, 0,
                SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

            // Query the actual window size now that Windows has natively scaled it.
            CRect exactScaledRect = m_dialog.GetWindowRect();
            int finalWidth = exactScaledRect.Width();
            int finalHeight = exactScaledRect.Height();

            // Compute bottom-right docking coordinates using the precise native
            // dimensions.
            int left = frameRect.right - finalWidth;
            int top = frameRect.bottom - finalHeight;

            // Move it to the final calculated coordinates and display it cleanly.
            m_dialog.SetWindowPos(HWND_TOPMOST, left, top, finalWidth, finalHeight, SWP_SHOWWINDOW);
        }
    }
    catch (const CWinException& e)
    {
        CString error = "Error reported by GetLastError:\n";
        error << e.GetErrorString();
        MessageBox(error, L"Failed to create dialog", MB_OK);
    }
}

// All window messages for this window pass through WndProc.
LRESULT CDialogHolder::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
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