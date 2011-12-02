//////////////////////////////////////////////////
// MyTaskDialog.cpp

#include "stdafx.h"
#include "MyTaskDialog.h"


BOOL CMyTaskDialog::OnTDButtonClicked(int nButtonID)
// Called when the user selects a button or command link.
{ 
	UNREFERENCED_PARAMETER(nButtonID);
	TRACE("Button or command link pressed\n");
	
	// we could return TRUE to prevent the task dialog from closing
	return FALSE;
}

void CMyTaskDialog::OnTDConstructed()
// Called when the task dialog is constructed, before it is displayed.
{
	TRACE("Task Dialog Constructed\n");
}

void CMyTaskDialog::OnTDCreated()
// Called when the task dialog is displayed.
{
	TRACE("Task Dialog Created\n");
}

void CMyTaskDialog::OnTDDestroyed()
// Called when the task dialog is destroyed.
{
	TRACE("Task Dialog Destroyed\n");
}

void CMyTaskDialog::OnTDExpandButtonClicked(BOOL bExpanded)
// Called when the expand button is clicked.
{
	UNREFERENCED_PARAMETER(bExpanded);
	TRACE("Expand Button Pressed\n");
}

void CMyTaskDialog::OnTDHelp()
// Called when the user presses F1 on the keyboard.
{
	TRACE("F1 Pressed\n");
}

void CMyTaskDialog::OnTDHyperlinkClicked(LPCTSTR pszHref)
// Called when the user clicks on a hyperlink.
{
	UNREFERENCED_PARAMETER(pszHref);
	TRACE("Hyperlink clicked\n");
}

void CMyTaskDialog::OnTDNavigatePage()
// Called when a navigation has occurred.
{
	TRACE("Navigation occurred\n");
}

BOOL CMyTaskDialog::OnTDRadioButtonClicked(int nRadioButtonID)
// Called when the user selects a radio button.
{
	UNREFERENCED_PARAMETER(nRadioButtonID);
	TRACE("Radio Button Clicked\n");
	return TRUE; 
}

BOOL CMyTaskDialog::OnTDTimer(DWORD dwTickCount)  
// Called every 200 milliseconds (aproximately) when the TDF_CALLBACK_TIMER flag is set. 
{
	UNREFERENCED_PARAMETER(dwTickCount);
	static int i = 0;
	if (i <= 100)
		SetProgressBarPosition(++i);

	return FALSE;
}

void CMyTaskDialog::OnTDVerificationCheckboxClicked(BOOL bChecked)
// Called when the user clicks the Task Dialog verification check box.
{
	UNREFERENCED_PARAMETER(bChecked);
	TRACE("Verification Checkbox Clicked\n");
}

LRESULT CMyTaskDialog::TaskDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	return TaskDialogProcDefault(uMsg, wParam, lParam);
}