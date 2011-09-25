///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
}

CMyDialog::~CMyDialog()
{
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Attach the dialog items to the CWnd objects
	AttachItem(IDC_PROGRESS1, m_ProgressBar);
	AttachItem(IDC_SCROLLBAR1, m_ScrollBar);
	AttachItem(IDC_SLIDER1, m_Slider);

	return true;
}

void CMyDialog::OnOK()
{
	MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
	CDialog::OnOK();
}

void CMyDialog::SetProgress(int nPos)
{
	m_ProgressBar.SetProgress(nPos);
}

void CMyDialog::SetScroll(int nPos)
{
	m_ScrollBar.SetScroll(nPos);
}

void CMyDialog::SetSlider(int nPos)
{
	m_Slider.SetPos(nPos, TRUE);
}

void CMyDialog::SetStatic(BOOL IsSlider, int nPos)
{
	TCHAR szStatic[80];
	if (IsSlider)
		wsprintf(szStatic, _T("Slider Position %d"), nPos);
	else
		wsprintf(szStatic, _T("Scroll Position %d"), nPos);

	// Set the text in the static control
	SetDlgItemText(IDC_STATIC2, szStatic);
}

