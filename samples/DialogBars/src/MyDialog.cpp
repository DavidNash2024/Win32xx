///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

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

void CMyDialog::OnOK()
{
    MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
    CDialog::OnOK();
}

void CMyDialog::SetProgress(int pos)
{
    m_progressBar.SetProgress(pos);
}

void CMyDialog::SetScroll(int pos)
{
    m_scrollBar.SetScroll(pos);
}

void CMyDialog::SetSlider(int pos)
{
    m_slider.SetPos(pos, TRUE);
}

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

