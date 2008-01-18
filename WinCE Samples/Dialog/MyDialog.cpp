
#include "MyDialog.h"
#include "resource.h"



CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent)
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
	::MessageBox(NULL, TEXT("DONE Button Pressed.  Program will exit now."), TEXT("Button"), MB_OK);
	CDialog::OnOK();
}

BOOL CMyDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
// Add any messages to be handled inside the switch statement


//	switch (uMsg)
//	{
//
//	} // switch(uMsg)
	
	return DialogProcDefault(hwnd, uMsg, wParam, lParam);
	
} // BOOL CALLBACK DialogProc(...)


