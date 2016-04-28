/* (10-08-2014) [Tab/Indent: 8/8][Line/Box: 80/74]            (CtlColorChoice.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a generic CtlColorChoice popup
	dialog class for applications using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein. Information on the use of this class appears in the
	CtlColorChoice.cpp file. This class extends the CColorChoice class to
	permit the alteration of colors for all control types and individual
	controls appearing on a frame dialog application. Instructions and
	restrictions on the usage of this class appear in CtlColorChoice.cpp.

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

	Declaration of the CtlColorChoice class

*******************************************************************************/

#ifndef CTLCOLORCHOICE_H
#define CTLCOLORCHOICE_H

#include "ColorDefs.h"
#include "CListBoxDlg.h"
#include "CColorChoice.h"

/*******************************************************************************

	Local struct and enum definitions                               */

   // Mnemonic identifiers for indexing the m_ColorTable array.  The ctl_color
   // pairs should be entered into this table in the order given
enum CtlColors
{
	DlgTxFg = 0, 	DlgTxBg, DlgBg,	// dialog
	BtnTxFg, 	BtnTxBg, BtnBg, // button
	EdtTxFg, 	EdtTxBg, EdtBg, // edit
	LBxTxFg, 	LBxTxBg, LBxBg, // list box
	SclTxFg, 	SclTxBg, SclBg, // scroll
	StcTxFg, 	StcTxBg, StcBg, // static
	REdTxFg, 	REdTxBg, REdBg, // rich edit
	EndDefaultColors                // end control class colors
};

/******************************************************************************/
	class
CtlColorChoice   : public CColorChoice					/*

	A class to support the selection of the colors appearing in the controls 
	of an application dialog.
*-----------------------------------------------------------------------------*/
{
	public:
		CtlColorChoice();
		virtual ~CtlColorChoice(){}
		
		virtual void	GetContolColors(UINT nCtlColor, COLORREF& fg,
				    COLORREF& bg, CBrush& br);

	protected:
		virtual void    InitCtlDefaultColors();
		virtual BOOL 	OnInitDialog();
			void 	InitCustomColors() {}
		virtual	void	Serialize(CArchive &ar);

	private:

};
/*----------------------------------------------------------------------------*/
#endif  // CTLCOLORCHOICE_H
