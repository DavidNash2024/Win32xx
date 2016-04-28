/* (01-Oct-2015) [Tab/Indent: 8/8][Line/Box: 80/74]           (CListBoxDlg.cpp) *
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

	Special Conventions:

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Implementation of the CListBoxDlg class

*******************************************************************************/

  // Include the Resource IDs defined by Win32++, numbered 51 - 99.
  // These are prefixed by IDW_.
#include "stdafx.h"
#include "CListBoxDlg.h"
#include "limits.h"
#include "resource.h"

/*============================================================================*/
	CListBoxDlg::
CListBoxDlg(HWND hParentWnd /* = 0 */)             			/*

	CListBoxDlg constructor: uses the IDD_CLISTVIEWBOX_DIALOG dialog
	defined in CListBoxDlg.rc
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
	    case WM_COMMAND:
	    {
		switch (LOWORD(wParam))
		{
		    case IDM_HELP_LIST_BOX:
			  // route the help message to the owner window
			OnHelpButton();
			return TRUE;
		}
	    }

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
	INT_MAX. Attach the box to the given hWndOwner if non-zero, or to the
	m_hWndOwner otherwise.  If the latter is also 0, attach the box to
	the main window.
*-----------------------------------------------------------------------------*/
{
	  // make sure there is a parent window
	if (hWndOwner != 0)
		m_hWndOwner = hWndOwner;
	if (m_hWndOwner == 0)
		m_hWndOwner = GetApp().GetMainWnd();
	int index = (int) CDialog::DoModal(m_hWndOwner);
	  // NOTE: the CDialog class automatically passes execution control to
	  // either the OnOK() or OnCancel() method upon termination of the
	  // modal dialog before returning control back here. The nResult value
	  // returned by CDialog::DoModal() is the parameter passed to a
	  // CDialog::EndDialog(nResult) statement; hence, on success, when the
	  // OnOK() method is invoked, it sets this return value to the index
	  // of the selected item in the list box, if one was selected. If no
	  // item was highlighted or the selection was cancelled, the OnCancel()
	  // method is invoked, which returns the INT_MAX value to indicate this
	  // condition. Had a -1 been attempted, as CDialog::EndDialog(-1), that
	  // procedure would have produced an unwanted assert(0). See OnOK() and
	  // OnCancel() for details.
	::UpdateWindow(hWndOwner);
	return (index < INT_MAX ? index : -1);
}

/*============================================================================*/
	void CListBoxDlg::
OnHelpButton()                                                          /*

	Override base class member to send the font context help identifier to
	the GW_OWNER window of this object.
*-----------------------------------------------------------------------------*/
{
	  // route the help message to the owner window
	HWND hwndOwner = GetWindow(GW_OWNER);
	if (::IsWindow(hwndOwner))
	{
		SendMessage(hwndOwner, WM_COMMAND,
		    IDC_HELP_COMDLG, IDM_HELP_LIST_BOX);
	}

}

/*============================================================================*/
	void CListBoxDlg::
OnOK()									/*

	This member executes after the DoModal() member terminates if the OK
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

	This member executes after the DoModal() member terminates and either
	the Cancel or [X] button on the dialog was activated. Since the
	CDialog::DoModal() method returns the parameter sent in the EndDialog()
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
	HWND hWndCtrl = ::GetDlgItem((HWND)*this, IDC_LIST_BOX);
	m_ListBox.Attach(hWndCtrl);
	m_ListBox.ResetContent();
	for (UINT i = 0; i < m_sList.size(); i++)
		m_ListBox.AddString(m_sList[i]);
	  // put the popup in a good location
	CenterWindow();

	return TRUE;
}


