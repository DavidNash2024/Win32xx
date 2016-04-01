/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]         (CustomButton.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: The CustomButton class implementation. This class
	is derived from the CButton class found in the Win32++ Windows interface
	classes, Copyright (c)2005-2015 David Nash, under permissions granted
	therein. It permits individual settings of the parameters of owner-drawn
	button controls on dialog forms.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
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

********************************************************************************

	Implementation of the CustomButton class

*******************************************************************************/

#include "stdafx.h"
#include "CustomButton.h"

/*============================================================================*/
	CustomButton::
CustomButton()               						/*

	Construct an owner-drawn button.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	void CustomButton::
DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)               		/*

	Custom draw the item specified in lpDrawItemStruct: here a button.
	Invoked by the view for the owner-drawn button in response to the
	WM_DRAWITEM system message. An owner-drawn button has the BS_OWNERDRAW
	style set. Also see the BS_ style values.
	
	Note: this procedure can show the focus rectangle of the button when the
	button has focus. Howeever, the focus does not appear to be cancelled
	when another control recieves the focus, and the non-owner-draw form
	of push buttons does not have this rectangle present when pressed
*-----------------------------------------------------------------------------*/
{
	  // get the device context and attach the button handle to it
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC); 
	  // determine the button rectangle
	CRect rect = lpDrawItemStruct->rcItem;
	  // draw in the button text
	dc.DrawText(GetWindowText(), -1, rect, DT_CENTER | DT_VCENTER |
	    DT_SINGLELINE);
	  // get the current state of the button
	UINT state = lpDrawItemStruct->itemState;
	if ( (state & ODS_SELECTED) )	// if it is pressed
	{
		dc.DrawEdge(rect, EDGE_SUNKEN, BF_RECT); // draw a sunken face
	}
	else
	{
		dc.DrawEdge(rect, EDGE_RAISED, BF_RECT); // draw a raised face
	}
	  // draw the focus rectangle, a dotted rectangle just inside the
	  // button rectangle when the button has the focus.
	if (lpDrawItemStruct->itemAction & ODA_FOCUS)
	{
		int iChange = 3;
		rect.top    += iChange;
		rect.left   += iChange;
		rect.right  -= iChange;
		rect.bottom -= iChange;
		dc.DrawFocusRect(rect);
	}
	dc.Detach();
}

/*============================================================================*/
	LRESULT CustomButton::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)			/*

	This method processes all messages sent to the button control. The uMsg
	parameter specifies the message and both wParam and lParam specify
	message-specific information not of importance to this class, but
	which is relayed on to the window default process. 
*-----------------------------------------------------------------------------*/
{
//	switch (uMsg)
//	{
//		// Add case statements for each message to be handled here
//
//	    default:
//	    	break;
//	}

	  // pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}



