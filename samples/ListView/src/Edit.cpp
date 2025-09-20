/////////////////////////////
// Edit.cpp
//

#include "stdafx.h"
#include "Edit.h"
#include "UserMessages.h"

/////////////////////////////
// CMyEdit function definitions
//

// Called when the edit control receives a character to process.
LRESULT CMyEdit::OnChar(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (wparam)
    {
        case VK_RETURN:
        {
            // Hides the edit window when the return key is pressed.
            ShowWindow(SW_HIDE);

            // Send a message to the list-view to update the subitem's text.
            GetParent().SendMessage(UWM_UPDATETEXT);

            // Return 0 to avoid a beep.
            return 0;
        }
        case VK_ESCAPE:
        {
            // Hides the edit window when the escape key is pressed.
            ShowWindow(SW_HIDE);

            // Return 0 to avoid a beep.
            return 0;
        }
    }

    // Perform default processing for this message.
    return FinalWindowProc(msg, wparam, lparam);
}

// Called the edit window loses keyboard focus.
LRESULT CMyEdit::OnKillFocus()
{
    ShowWindow(SW_HIDE);
    return 0;
}

// Called before the edit control is created.
void CMyEdit::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_CHILD | ES_AUTOHSCROLL;
}

LRESULT CMyEdit::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_KILLFOCUS:  return OnKillFocus();
        case WM_CHAR:       return OnChar(msg, wparam, lparam);
        }

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
