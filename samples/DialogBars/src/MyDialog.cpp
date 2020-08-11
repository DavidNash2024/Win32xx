/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


/////////////////////////////////
// CMyDialog function definitions
//

// Constructor.
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

// Destructor.
CMyDialog::~CMyDialog()
{
}

// Called when the dialog window is destroyed.
void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach the dialog items to the CWnd objects
    AttachItem(IDC_PROGRESS1, m_progressBar);
    AttachItem(IDC_SCROLLBAR1, m_scrollBar);
    AttachItem(IDC_SLIDER1, m_slider);

    return TRUE;
}

// Called when the OK button or Enter key is pressed.
void CMyDialog::OnOK()
{
    MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
    CDialog::OnOK();
}

// Sets the progress bar's position.
void CMyDialog::SetProgress(int pos)
{
    m_progressBar.SetProgress(pos);
}

// Sets the scroll bar's position.
void CMyDialog::SetScroll(int pos)
{
    m_scrollBar.SetScroll(pos);
}

// Sets the slider control's position.
void CMyDialog::SetSlider(int pos)
{
    m_slider.SetPos(pos, TRUE);
}

// Sets the static control's text.
void CMyDialog::SetStatic(BOOL isSlider, int pos)
{
    CString str;
    if (isSlider)
        str.Format(_T("Slider Position %d"), pos);
    else
        str.Format(_T("Scroll Position %d"), pos);

    // Set the text in the static control
    SetDlgItemText(IDC_STATIC2, str);
}

