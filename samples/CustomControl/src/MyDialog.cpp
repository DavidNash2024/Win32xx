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
    ::PostQuitMessage(0);
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Pass resizing messages on to the resizer
    m_resizer.HandleMessage(msg, wparam, lparam);

//  switch (uMsg)
//  {
//      //Additional messages to be handled go here
//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam); 
}

void CMyDialog::OnCancel()
{
    // Suppress default handling of OnCancel.
    // Called when the esc button is pressed.
}

void CMyDialog::OnOK()
{
    TRACE("Enter key pressed\n");
    // Send Ctrl M to the browser

    // Create a generic keyboard event structure

#ifdef INPUT_KEYBOARD   // Not supported on some old ms compilers

    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(ip));

    // Press the "M" key
    ip.ki.wVk = 'M';
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(ip));
    
    // Release the "M" key
    ip.ki.wVk = 'M';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(ip)); 
    
    // Release the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(ip));

#endif
}

BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach the custom control to a CWnd object
    m_webControl.AttachDlgItem(IDC_CUSTOMWEB, *this);

    // Support automatic resizing
    m_resizer.Initialize( *this, CRect(0, 0, 300, 200) );
    m_resizer.AddChild(m_webControl, topleft, RD_STRETCH_WIDTH|RD_STRETCH_HEIGHT);
    
    return TRUE;
}



