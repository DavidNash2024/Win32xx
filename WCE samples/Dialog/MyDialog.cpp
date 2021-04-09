/////////////////////////////
// MyDialog.cpp


#include "MyDialog.h"
#include "resource.h"


CMyDialog::CMyDialog(UINT resID)
    : CDialog(resID)
{
}

CMyDialog::~CMyDialog()
{
}


BOOL CMyDialog::OnInitDialog()
{
    // This function is called before the dialog is displayed.


    //Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    return true;
}

void CMyDialog::OnOK()
{
    ::MessageBox(0, TEXT("DONE Button Pressed.  Program will exit now."), TEXT("Button"), MB_OK);
    CDialog::OnOK();
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
// Add any messages to be handled inside the switch statement


//  switch (msg)
//  {
//
//  } // switch(msg)

    return DialogProcDefault(msg, wparam, lparam);

} // INT_PTR CALLBACK DialogProc(...)


