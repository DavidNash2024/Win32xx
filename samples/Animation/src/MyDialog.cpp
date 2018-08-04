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

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
        //Additional messages to be handled go here
//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    AttachItem(IDC_ANIMATE1, m_animation);

    m_animation.Open(MAKEINTRESOURCE(IDA_UPLOAD_AVI));
    m_animation.Play(0, -1, -1);

    return TRUE;
}


