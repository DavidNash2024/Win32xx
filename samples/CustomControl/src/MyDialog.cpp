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

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass resizing messages on to the resizer
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

//	switch (uMsg)
//	{
//		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam); 
}

void CMyDialog::OnOK()
{
	TRACE("Enter key pressed\n");
	// Send Ctrl M to the browser

	// Create a generic keyboard event structure

#ifdef INPUT	// Not supported on some old ms compilers

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the "Ctrl" key
	ip.ki.wVk = VK_CONTROL;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	// Press the "M" key
	ip.ki.wVk = 'M';
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));
	
	// Release the "M" key
	ip.ki.wVk = 'M';
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT)); 
	
	// Release the "Ctrl" key
	ip.ki.wVk = VK_CONTROL;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

#endif
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Attach the custom control to a CWnd object
	m_MyControl.AttachDlgItem(IDC_CUSTOMWEB, this);

	// Support automatic resizing
	m_Resizer.Initialize( this, CRect(0, 0, 300, 200) );
	m_Resizer.AddChild(m_MyControl, topleft, RD_STRETCH_WIDTH|RD_STRETCH_HEIGHT);
	
	return TRUE;
}



