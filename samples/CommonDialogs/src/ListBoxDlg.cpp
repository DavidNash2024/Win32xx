/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]           (ListBoxDlg.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of a generic CListBoxDlg popup
	dialog class for applications using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Special Conventions:

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CListBoxDlg class

*******************************************************************************/

  // Include the Resource IDs defined by Win32++, numbered 51 - 99.
  // These are prefixed by IDW_.

#include "stdafx.h"
#include "default_resource.h"
#include "ListBoxDlg.h"
#include "ListBoxDlgRC.h"

/*============================================================================*/
	CListBoxDlg::
CListBoxDlg(HWND hParentWnd /* = 0 */)             			/*

	CListBoxDlg constructor: uses the IDD_CLISTVIEWBOX_DIALOG dialog
	defined in ListBoxDlg.rc
*-----------------------------------------------------------------------------*/
    :   CCommonDialog(IDD_CLISTVIEWBOX_DIALOG)
{
	m_hWndOwner = hParentWnd;
	SetBoxTitle(_T("List"));
	AddListItem(_T("default item 0"));
}

/*============================================================================*/
	INT_PTR CListBoxDlg::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)			/*

	This is the list box message loop. It handles those messages specific
	to this derived class.  Messages for actions common to all dialogs,
	including normal housekeeping functionsm are relegated to the base
	class default handler. Here it also handles the closing of the dialog
	via [X] to produce the same results as the Cancel button on the dialog.
*-----------------------------------------------------------------------------*/
{
	switch (uMsg)
	{
//	    case WM_COMMAND:
//	    {
//		switch (LOWORD(wParam))
//		{
//		}
//	    }
	    
	    case WM_SYSCOMMAND:
	    {
		switch (LOWORD(wParam))
		{
		    case SC_CLOSE:
			OnCancel();
			return TRUE;
		}
	    }
	}

	  // pass unhandled messages on for default processing
	return DialogProcDefault(uMsg, wParam, lParam);
}

/*============================================================================*/
	INT_PTR CListBoxDlg::
DoModal(HWND hWndOwner /* = 0 */)      					/*

	Invoke the modal dialog box and return the index of the selected item
	in the list box when done. The user cannot interact with other windows
	until the dialog box is closed, which happens on selection of the OK,
	Cancel, or [X] button on the dialog. If OK is selected, return the index
	of the item highlighted in the list box. Otherwise, return the value
	-1. Attach the box to the given hWndOwner if non-zero, or to the
	m_hWndOwner otherwise.  If the latter is also 0, attach the box to
	the main window.
*-----------------------------------------------------------------------------*/
{
	  // make sure there is a parent window
	if (hWndOwner != 0)
		m_hWndOwner = hWndOwner;
	if (m_hWndOwner == 0)
		m_hWndOwner = GetApp().GetMainWnd();
	int ok = static_cast<int>(CDialog::DoModal(m_hWndOwner));
	  // NOTE: the CDialog class message loop handles the passage of
	  // execution to OnOK() or OnCancel(), the ending of the dialog, and
	  // the determination of the return value. Since the CDialog::()
	  // method returns the parameter sent in the CDialog::EndDialog()
	  // statement, the OnOK() member sets the return value to the index of
	  // the selected item in the list box, if one was selected. However, if
	  // no item is highlighted in the list box, the native value returned
	  // is -1, which, if sent to EndDialog(), produces an unwanted
	  // assert(0). In this case, INT_MAX is returned instead. See OnOK()
	  // and OnCancel() for the mechanism that implements this behavior.
	::UpdateWindow(hWndOwner);
	return (ok < INT_MAX ? ok : -1);
}

/*============================================================================*/
	void CListBoxDlg::
OnOK()									/*

	This member executes after the () member terminates if the OK
	button was activated.
*-----------------------------------------------------------------------------*/
{
    	int choice = m_ListBox.GetCurSel();
	  // return the choice: see the note above
	CDialog::EndDialog((INT_PTR) choice >= 0 ? choice : INT_MAX);
}

/*============================================================================*/
	void CListBoxDlg::
OnCancel()								/*

	This member executes after the () member terminates and either
	the Cancel or [X] button on the dialog was activated. Since the
	CDialog::() method returns the parameter sent in the EndDialog()
	statement, this member sets that value to INT_MAX so that no unwanted
	assert(0) occurs.
*-----------------------------------------------------------------------------*/
{
	CDialog::EndDialog((UINT_PTR)INT_MAX);
}

/*============================================================================*/
	BOOL  CListBoxDlg::
OnInitDialog()								/*

	Set the dialog banner text and list box contents to values prestored
	in the list box string list. Locate the list box in the center of the
	window.
*-----------------------------------------------------------------------------*/
{
	  // do the base class initialization first
	CDialog::OnInitDialog();
	SetWindowTitle();
	  // subclass the dialog controls
	HWND hWndCtrl = ::GetDlgItem(*this, IDC_LIST_BOX);
	m_ListBox.Attach(hWndCtrl);
	m_ListBox.ResetContent();
	for (UINT i = 0; i < m_sList.size(); i++)
		m_ListBox.AddString(m_sList[i]);
	  // put the popup in a good location
	CenterWindow();

	return TRUE;
}


