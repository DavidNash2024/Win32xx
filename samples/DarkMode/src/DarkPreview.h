/////////////////////////////
// DarkPreview.h
//

#ifndef _DARK_PREVIEW_H_
#define _DARK_PREVIEW_H_


////////////////////////////////////////////////////////////////////
// CDarkPreview is a class template used to provides a print preview
// that is compatible with dark mode.
//
template <typename T>
class CDarkPreview : public CPrintPreview<T>
{
public:
    CDarkPreview() : m_darkMode(false) {}
    virtual ~CDarkPreview() {}

    INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnCtlColors(UINT, WPARAM wparam, LPARAM);
    LRESULT OnDrawItem(WPARAM, LPARAM lparam);
    BOOL OnInitDialog();
    void SetButtonOwnerDraw(bool isOwnerDraw);
    void SetDarkMode(bool darkMode);

private:
    CDarkPreview(const CDarkPreview&);                // Disable copy construction
    CDarkPreview& operator = (const CDarkPreview&);   // Disable assignment operator

    bool m_darkMode;
};


////////////////////////////////////////////////////
// Definitions for the CDarkPreview class template
// member functions.
//

// Handle the dialogs window messages.
template <class T>
inline INT_PTR CDarkPreview<T>::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORSTATIC:  return OnCtlColors(msg, wparam, lparam);
        case WM_DRAWITEM:        return OnDrawItem(wparam, lparam);
        }

        // Pass unhandled messages on to parent DialogProc.
        return CPrintPreview<T>::DialogProc(msg, wparam, lparam);
    }

    // catch all CException types
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Set the colors for the dialog and static controls.
template <class T>
inline LRESULT CDarkPreview<T>::OnCtlColors(UINT, WPARAM wparam, LPARAM)
{
    if (m_darkMode)
    {
        HDC dc = (HDC)wparam;
        ::SetBkMode(dc, TRANSPARENT);
        ::SetTextColor(dc, RGB(255, 255, 255));

        return reinterpret_cast<LRESULT>(::GetStockObject(BLACK_BRUSH));
    }

    return 0;
}

// Perform the owner draw for buttons.
template <class T>
inline LRESULT CDarkPreview<T>::OnDrawItem(WPARAM, LPARAM lparam)
{
    LPDRAWITEMSTRUCT pDraw = (LPDRAWITEMSTRUCT)lparam;
    CDC dc(pDraw->hDC);
    CRect rect = pDraw->rcItem;
    dc.SetTextColor(RGB(220, 220, 220));

    if (pDraw->itemState & ODS_SELECTED)
    {
        dc.CreatePen(PS_SOLID, 1, RGB(140, 140, 140));
        dc.CreateSolidBrush(RGB(60, 60, 60));
    }
    else
    {
        dc.CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
        dc.CreateSolidBrush(RGB(80, 80, 80));
    }

    // Draw the button text.
    CString str = this->GetDlgItemText(pDraw->CtlID);
    rect.DeflateRect(1, 1);
    dc.RoundRect(rect, 10, 10);
    dc.DrawText(str, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    return TRUE;
}

// Called before the dialog is displayed.
template <class T>
inline BOOL CDarkPreview<T>::OnInitDialog()
{
    // Set the application icon
    CPrintPreview<T>::OnInitDialog();

    // Make the OK button owner drawn for dark mode.
    SetButtonOwnerDraw(m_darkMode);

    return TRUE;
}

// Use owner draw for buttons for dark mode.
template <class T>
inline void CDarkPreview<T>::SetButtonOwnerDraw(bool isOwnerDraw)
{
    HWND print  = this->GetDlgItem(IDW_PREVIEWPRINT);
    HWND setup  = this->GetDlgItem(IDW_PREVIEWSETUP);
    HWND prev   = this->GetDlgItem(IDW_PREVIEWPREV);
    HWND next   = this->GetDlgItem(IDW_PREVIEWNEXT);
    HWND close  = this->GetDlgItem(IDW_PREVIEWCLOSE);

    DWORD style = static_cast<DWORD>(::GetWindowLongPtr(print, GWL_STYLE));

    if (isOwnerDraw)
    {
        ::SetWindowLongPtr(print, GWL_STYLE, style | BS_OWNERDRAW);
        ::SetWindowLongPtr(setup, GWL_STYLE, style | BS_OWNERDRAW);
        ::SetWindowLongPtr(prev,  GWL_STYLE, style | BS_OWNERDRAW);
        ::SetWindowLongPtr(next,  GWL_STYLE, style | BS_OWNERDRAW);
        ::SetWindowLongPtr(close, GWL_STYLE, style | BS_OWNERDRAW);
    }
    else
    {
        ::SetWindowLongPtr(print, GWL_STYLE, style & ~BS_OWNERDRAW);
        ::SetWindowLongPtr(setup, GWL_STYLE, style & ~BS_OWNERDRAW);
        ::SetWindowLongPtr(prev,  GWL_STYLE, style & ~BS_OWNERDRAW);
        ::SetWindowLongPtr(next,  GWL_STYLE, style & ~BS_OWNERDRAW);
        ::SetWindowLongPtr(close, GWL_STYLE, style & ~BS_OWNERDRAW);
    }
}

// Configure the dialog for dark mode.
template <class T>
inline void CDarkPreview<T>::SetDarkMode(bool darkMode)
{
    // Make the buttons owner drawn for dark mode.
    if (this->IsWindow())
        SetButtonOwnerDraw(darkMode);

    m_darkMode = darkMode;
}


#endif // _DARK_PREVIEW_H_
