//////////////////////////////////////////////
// TaskDialogApp.cpp

#include "stdafx.h"
#include "TaskDialogApp.h"
#include "MyTaskDialog.h"
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
	::InitCommonControls();

	// Create the TaskDialog object
	if (!CTaskDialog::IsSupported())
	{
		// Task Dialogs are only supported on Vista and above
		MessageBox(NULL, _T("Vista or better is required for Task Dialogs"), _T("Not Supported!"), MB_ICONERROR);	
		return FALSE;
	}

	CMyTaskDialog td;
	
	td.SetOptions( TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS | TDF_EXPANDED_BY_DEFAULT | TDF_SHOW_PROGRESS_BAR | TDF_CALLBACK_TIMER);
	
	// Add the buttons
	td.AddCommandControl( CB_FIRST, MAKEINTRESOURCE(IDS_CB_FIRST) );
	td.AddCommandControl( CB_SECOND, MAKEINTRESOURCE(IDS_CB_SECOND) );
	td.AddCommandControl( CB_THIRD, MAKEINTRESOURCE(IDS_CB_THIRD) );
	td.AddRadioButton( RB_FIRST,  MAKEINTRESOURCE(IDS_RB_FIRST) );
	td.AddRadioButton( RB_SECOND, MAKEINTRESOURCE(IDS_RB_SECOND) );
	td.AddRadioButton( RB_THIRD,  MAKEINTRESOURCE(IDS_RB_THIRD) );
	td.SetDefaultRadioButton( RB_FIRST );
	td.SetCommonButtons( TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON );

	// Add the Text
	td.SetWindowTitle( MAKEINTRESOURCE(IDS_WINDOWTITLE) );
	td.SetMainInstruction( MAKEINTRESOURCE(IDS_MAININSTRUCTION) );
	td.SetContent( MAKEINTRESOURCE(IDS_CONTENT) );
	td.SetExpansionArea( MAKEINTRESOURCE(IDS_EXPANDED), _T("Hide the expanded information"), _T("Show the expanded information") );
	td.SetVerificationCheckboxText( MAKEINTRESOURCE(IDS_VERIFICATIONTEXT) );
	td.SetFooterText( MAKEINTRESOURCE(IDS_FOOTER) );

	// Set Icons
	td.SetMainIcon(TD_INFORMATION_ICON);
	td.SetFooterIcon(TD_INFORMATION_ICON);

	// Run the task dialog
	HRESULT hr = td.DoModal();

	// Respond to the result
	if (SUCCEEDED(hr))
	{
		switch (td.GetSelectedButtonID())
		{
		case CB_FIRST:
			TRACE("First command control selected\n");
			break;
		case CB_SECOND:
			TRACE("Second command control selected\n");
			break;
		case CB_THIRD:
			TRACE("Third command control selected\n");
			break;
		case IDYES:
			TRACE("The 'Yes' button was pressed\n");
			break;
		case IDNO:
			TRACE("The 'No' button was pressed\n");
			break;
		case IDCANCEL:
			TRACE("The 'Cancel' button was pressed\n");
			break;
		}

		switch (td.GetSelectedRadioButtonID())
		{
		case RB_FIRST:
			TRACE("First radio button selected\n");
			break;
		case RB_SECOND:
			TRACE("Second radio button selected\n");
			break;
		case RB_THIRD:
			TRACE("Third radio button selected\n");
			break;
		}

		if (td.GetVerificationCheckboxState())
		{
			TRACE("Verification button selected\n");
		}
	}
	else
	{
	   TRACE("Some error occurred\n");
	}

	return FALSE;	// Don't run the message loop
}


