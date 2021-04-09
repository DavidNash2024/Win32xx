/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"

/////////////////////////////////
// CMyDialog finction definitions
//

// Constructor
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

// Destructor
CMyDialog::~CMyDialog()
{
}

// Called when the dialog window is destroyed.
void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

// Handles the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Additional messages to be handled go here
    //  }

        // Pass unhandled messages on to parent DialogProc
        return DialogProcDefault(msg, wparam, lparam);
    }

    // catch all CException types
    catch (const CException& e)
    {
        // Display the exception and continue
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach the animation control to our CAnimation member variable.
    AttachItem(IDC_ANIMATE1, m_animation);

    // Play the animation.
    m_animation.Open(MAKEINTRESOURCE(IDA_UPLOAD_AVI));
    m_animation.Play(0, -1, -1);

    return TRUE;
}


