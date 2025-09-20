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
// in the bottom right corner of the frame.
void CDialogHolder::ShowDialog(CWnd* pFrame, unsigned char* dlgArray)
{
    assert(pFrame);
    assert(dlgArray);

    // Destroy the current dialog and dialog holder.
    m_dialog.Destroy();
    Destroy();

    // Create the dialog holder window.
    Create();
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);
    SetWindowText(L"Dialog Holder");

    try
    {
        // Create the dialog.
        m_dialog.SetDialogTemplate((LPCDLGTEMPLATE)dlgArray);
        m_dialog.Create(*this);  // Creates a modeless dialog

        // Calculate the bottom right position of the frame.
        CRect dlgRect = m_dialog.GetWindowRect();
        CRect frameRect = pFrame->GetWindowRect();
        int left = frameRect.left + frameRect.Width() - dlgRect.Width();
        int top = frameRect.top + frameRect.Height() - dlgRect.Height();
        int width = dlgRect.Width();
        int height = dlgRect.Height();

        // Position the dialog holder and dialog.
        DWORD style = m_dialog.GetStyle();
        if (style & WS_CHILD)
        {
            int captionHeight = GetWindowRect().Height() - GetClientRect().Height();
            height += captionHeight;
            top -= captionHeight;
            SetWindowPos(HWND_TOPMOST, left, top, width, height, SWP_SHOWWINDOW);

            // Reposition the dialog at top left corner of the dialog holder.
            m_dialog.SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);
        }
        else
        {
            SetWindowPos(HWND_TOP, left, top, width, height, 0);
            m_dialog.SetWindowPos(HWND_TOPMOST, left, top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
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