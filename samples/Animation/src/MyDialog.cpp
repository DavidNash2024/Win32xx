///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID) : CDialog(nResID)
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

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	AttachItem(IDC_ANIMATE1, m_Animation);

	m_Animation.Open(MAKEINTRESOURCE(IDA_UPLOAD_AVI));
	m_Animation.Play(0, -1, -1);

	return TRUE;
}


