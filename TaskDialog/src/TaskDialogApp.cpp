//////////////////////////////////////////////
// TaskDialogApp.cpp

#include "stdafx.h"
#include "TaskDialogApp.h"
#include "Resource.h"

// Definitions for the CTaskDialogApp class
CTaskDialogApp::CTaskDialogApp()
{
}

CTaskDialogApp::~CTaskDialogApp()
{
}

BOOL CTaskDialogApp::InitInstance()
{
	// Create the TaskDialog object
	if (!CTaskDialog::IsSupported())
	{
		// Task Dialogs are only supported on Vista and above
		MessageBox(NULL, _T("Vista or better is required for Task Dialogs"), _T("Not Supported!"), MB_ICONERROR);	
		return FALSE;
	}

	CTaskDialog td;
	
	td.SetOptions(TDF_USE_HICON_MAIN | TDF_USE_HICON_FOOTER | TDF_ALLOW_DIALOG_CANCELLATION | TDF_EXPAND_FOOTER_AREA);
	
	// Add the buttons
	td.AddButton( CB_SAVE, MAKEINTRESOURCE(IDS_CB_SAVE) );
	td.AddRadioButton( RB_GOOD, MAKEINTRESOURCE(IDS_RB_GOOD) );
	td.AddRadioButton( RB_OK,   MAKEINTRESOURCE(IDS_RB_OK) );
	td.AddRadioButton( RB_BAD,  MAKEINTRESOURCE(IDS_RB_BAD) );
	td.SetDefaultRadioButton( RB_OK );
	td.SetCommonButtons( TDCBF_CANCEL_BUTTON );

	// Add the Text
	td.SetWindowTitle( MAKEINTRESOURCE(IDS_WINDOWTITLE) );
	td.SetMainInstruction( MAKEINTRESOURCE(IDS_MAININSTRUCTION) );
	td.SetContent( MAKEINTRESOURCE(IDS_CONTENT) );
	td.SetVerificationCheckboxText( MAKEINTRESOURCE(IDS_VERIFICATIONTEXT) );
	td.SetFooterText( MAKEINTRESOURCE(IDS_FOOTER) );

	// Run the task dialog
	HRESULT hr = td.DoModal();

	// Respond to the result
	if (SUCCEEDED(hr))
	{
		if (td.GetSelectedButtonID() == CB_SAVE)
		{
			switch (td.GetSelectedRadioButtonID())
			{
			case RB_GOOD:
				MessageBox(NULL, _T("TaskDialog Result"), _T("You like TaskDialogs alot"), MB_OK);
				break;

			case RB_OK:
				MessageBox(NULL, _T("TaskDialog Result"), _T("You like TaskDialogs a little bit"), MB_OK);  
				break;

			case RB_BAD:
				MessageBox(NULL, _T("TaskDialog Result"), _T("You don't like TaskDialogs at all"), MB_OK);
				break;
			}

			if (td.GetVerificationCheckboxState())
			{
			 //    the user checked the verification flag...
			 //    do any additional work here
			}
		}
		else
		{
		  //   user cancelled out of the dialog
		}
	}
	else
	{
	   //  some error occurred...check hr to see what it is
	}

	return FALSE;	// Don't run the message loop
}


