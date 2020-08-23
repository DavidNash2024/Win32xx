/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]             (MyButton.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The CMyButton class implementation. This class is
    derived from the CButton class found in the Win32++ Windows interface
    classes. It permits individual settings of the parameters of owner-drawn
    button controls on dialog forms.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "MyButton.h"

/*============================================================================*/
    void CMyButton::
DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)                     /*

    Custom draw the item specified in lpDrawItemStruct: here a button.
    Invoked by the view for the owner-drawn button in response to the
    WM_DRAWITEM system message. An owner-drawn button has the BS_OWNERDRAW
    style set. Also see the BS_ style values.
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
    if ( (state & ODS_SELECTED) )   // if it is pressed
        dc.DrawEdge(rect, EDGE_SUNKEN, BF_RECT); // draw a sunken face
    else
        dc.DrawEdge(rect, EDGE_RAISED, BF_RECT); // draw a raised face
      // draw the focus rectangle, a dotted rectangle just inside the
      // button rectangle when the button has the focus.
    if (lpDrawItemStruct->itemAction & ODA_FOCUS)
    {
        int iChange = 3;
        rect.top += iChange;
        rect.left += iChange;
        rect.right -= iChange;
        rect.bottom -= iChange;
        dc.DrawFocusRect(rect);
    }
    dc.Detach();
}
/*----------------------------------------------------------------------------*/
