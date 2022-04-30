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
    SetWindowText(_T("Dialog Holder"));

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
            m_dialog.SetWindowPos(0, 0, 0, 0, 0, SWP_NOSIZE);
        }
        else
        {
            SetWindowPos(0, left, top, width, height, 0);
            m_dialog.SetWindowPos(HWND_TOPMOST, left, top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        }
    }

    catch (CWinException& e)
    {
        CString error = "Error reported by GetLastError:\n";
        error << e.GetErrorString();
        MessageBox(error, _T("Failed to create dialog"), MB_OK);
    }
}
