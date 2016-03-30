/* (10-08-2014) [Tab/Indent: 8/8][Line/Box: 80/74]        (CtlColorChoice.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CtlColorChoice class for
	applications using the Win32++ Windows interface classes, Copyright
	(c) 2005-2016 David Nash, under permissions granted therein. This class
	extends the CColorChoice class to permit the alteration of colors for
	all control types and individual controls appearing on a frame dialog
	application. Instructions and restrictions on the usage of this class
	appear below.

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

	Usage Information:
serialize.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CtlColorChoice class

*******************************************************************************/

#include "stdafx.h"				// dn ...
#include "CtlColorChoice.h"

/*============================================================================*/
	CtlColorChoice::
CtlColorChoice()		                			/*

	Construct CtlColorChoice object and initialize custom and control
	default colors.
*-----------------------------------------------------------------------------*/
{
	InitCtlDefaultColors(); 
	SetBoxTitle(_T("Choose color for control"));
	GetListBoxDlg().SetBoxTitle(_T("Choose control to recolor"));
}

/*============================================================================*/
	void CtlColorChoice::
GetContolColors(UINT nCtlColor, COLORREF& fg, COLORREF& bk, CBrush& br)	/*

	Return the text foreground fg, text background bk, and control
	background brush br for the control class nCtlColor whose color
	values are found in the color table at the respective color indexes.
*-----------------------------------------------------------------------------*/
{
	switch (nCtlColor)
	{
	    case WM_CTLCOLORDLG:
	    	fg = GetColor(DlgTxFg);
	    	bk = GetColor(DlgTxBg);
    		br = GetBrush(DlgBg);
		break;

	    case WM_CTLCOLORBTN:
		fg = GetColor(BtnTxFg);
    		bk = GetColor(BtnTxBg);
    		br = GetBrush(BtnBg);
		break;

	    case WM_CTLCOLOREDIT:
	    	fg = GetColor(EdtTxFg);
	    	bk = GetColor(EdtTxBg);
    		br = GetBrush(EdtBg);
		break;

	    case WM_CTLCOLORLISTBOX:
	    	fg = GetColor(LBxTxFg);
	    	bk = GetColor(LBxTxBg);
    		br = GetBrush(LBxBg);
		break;

	    case WM_CTLCOLORSCROLLBAR:
	    	fg = GetColor(SclTxFg);
	    	bk = GetColor(SclTxBg);
    		br = GetBrush(SclBg);
		break;

	    case WM_CTLCOLORSTATIC:
	    	fg = GetColor(StcTxFg);
	    	bk = GetColor(StcTxBg);
    		br = GetBrush(StcBg);
		break;
		
	    default:
	    	fg = COLOR_BLACK;
		bk = COLOR_BLACK;
		br = GetBrush(COLOR_BLACK);
		break;
	}
}

/*============================================================================*/
	void CtlColorChoice::
InitCtlDefaultColors() 							/*

	Initialize the view color table entries.
*-----------------------------------------------------------------------------*/
{
	  // load default color table
	ClearColorTable();
	AddColorChoice(_T("o  Dialog text foreground"),     COLOR_WHITE);
	AddColorChoice(_T("o  Dialog text background"),     COLOR_BLUE);
	AddColorChoice(_T("o  Dialog background"), 	    COLOR_LT_BLUE);

	AddColorChoice(_T("o  Button text foreground"),     COLOR_BLACK);
	AddColorChoice(_T("o  Button text background"),     COLOR_CYAN);
	AddColorChoice(_T("o  Button background"), 	    COLOR_LT_CYAN);

	AddColorChoice(_T("o  Edit text foreground"), 	    COLOR_WHITE);
	AddColorChoice(_T("o  Edit text backgroound"), 	    COLOR_RED);
	AddColorChoice(_T("o  Edit control background"),    COLOR_LT_RED);

	AddColorChoice(_T("o  List box text foreground"),   COLOR_BLACK);
	AddColorChoice(_T("o  List box text background"),   COLOR_GREEN);
	AddColorChoice(_T("o  List box background"), 	    COLOR_LT_GREEN);

	AddColorChoice(_T("o  Scroll bar text foreground"), COLOR_WHITE);
	AddColorChoice(_T("o  Scroll bar text background"), COLOR_MAGENTA);
	AddColorChoice(_T("o  Scroll bar background"), 	    COLOR_LT_MAGENTA);

	AddColorChoice(_T("o  Static box text foreground"), COLOR_BLACK);
	AddColorChoice(_T("o  Static box text background"), COLOR_YELLOW);
	AddColorChoice(_T("o  Static box background"), 	    COLOR_LT_YELLOW);
	  // richedit controls (these are set differently than the others)
	AddColorChoice(_T("o  RichEdit text foreground"),   COLOR_WHITE);
	AddColorChoice(_T("o  RichEdit text background"),   COLOR_RED);
	AddColorChoice(_T("o  RichEdit background"), 	    COLOR_LT_RED);
}

/*============================================================================*/
	BOOL CtlColorChoice::
OnInitDialog()                                                          /*

	Perform any actions required for initialization of this object when
	the color choice is being initialized.
*-----------------------------------------------------------------------------*/
{
	  // do the base class initialization first (it currently does nothing)
	CDialog::OnInitDialog();
	SetWindowTitle();
	InitCustomColors();
	return TRUE;
}

/*============================================================================*/
        void CtlColorChoice::
Serialize(CArchive &ar)                                               	/*

        Called to serialize the current object to or from the archive ar,
	depending on the sense of IsStoring(). The entire color table is
	serialized by the base class. Any additional table entries that are
	made in this class here may require some remediation to prevent the
	application from re-adding these same elements into the table when the
	archived table has been recovered. Leaves the archive open for for
	further operations. Override this method to serialize the derived
	class.
*-----------------------------------------------------------------------------*/
{
	  // serialize the base class first
	CColorChoice::Serialize(ar);
	  // perform loading or storing
        if (ar.IsStoring())
        {
        	// add items to be saved here
  	}
        else    // recovering
        {
        	// add items to be recovered here
 	}
}

