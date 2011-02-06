
#include "stdafx.h"
#include "TaskDialog.h"
#include "resource.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	CWinApp MyApp;

	// Create the TaskDialog object
	CTaskDialog td;
	td.SetOptions(TDF_USE_HICON_MAIN | TDF_USE_HICON_FOOTER | TDF_ALLOW_DIALOG_CANCELLATION | TDF_EXPAND_FOOTER_AREA);
	
	// Add the buttons
	td.AddButton( CB_SAVE, MAKEINTRESOURCE(IDS_CB_SAVE) );
	td.AddRadioButton( RB_GOOD, MAKEINTRESOURCE(IDS_RB_GOOD) );
	td.AddRadioButton( RB_OK,   MAKEINTRESOURCE(IDS_RB_OK) );
    td.AddRadioButton( RB_BAD,  MAKEINTRESOURCE(IDS_RB_BAD) );
	td.SetDefaultRadioButton( RB_OK );
	td.SetCommonButtons(TDCBF_CANCEL_BUTTON);

	// Add the Text
	td.SetWindowTitle(MAKEINTRESOURCE(IDS_WINDOWTITLE));
	td.SetMainInstruction(MAKEINTRESOURCE(IDS_MAININSTRUCTION));
	td.SetContent(MAKEINTRESOURCE(IDS_CONTENT));
	td.SetVerificationCheckboxText(MAKEINTRESOURCE(IDS_VERIFICATIONTEXT));
	td.SetFooterText(MAKEINTRESOURCE(IDS_FOOTER));

	// Add the icons
	HICON hIconApp;
	LoadIconWithScaleDown(NULL, IDI_APPLICATION,
						  GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON),
						  &hIconApp);
	
	HICON hIconInfo;
	LoadIconWithScaleDown(NULL, IDI_INFORMATION,
						  GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
						  &hIconInfo);
	
	td.SetMainIcon(hIconApp);
	td.SetFooterIcon(hIconInfo);

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
				TaskDialog(NULL, NULL,
						   L"TaskDialog Result",
						   L"You like TaskDialogs alot", L"I'm glad you like TaskDialogs!",
						   TDCBF_OK_BUTTON, NULL, NULL);
				break;

			case RB_OK:
				TaskDialog(NULL, NULL,
						   L"TaskDialog Result",
						   L"You like TaskDialogs a little bit",  
						   L"Maybe we'll do better next time.",
						   TDCBF_OK_BUTTON, NULL, NULL);
				break;

			case RB_BAD:
				TaskDialog(NULL, NULL,
						   L"TaskDialog Result",
						   L"You don't like TaskDialogs at all", L"Back to MessageBox you go!",
						   TDCBF_OK_BUTTON, NULL, NULL);
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

	return 0;
}
