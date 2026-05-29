/////////////////////////////////////////
// MyButton.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "MyButton.h"

//////////////////////////////////
// CMyButton function definitions.
//

// Custom draw the button. This function is called in response to the
// WM_DRAWITEM system message. An owner drawn button has the BS_OWNERDRAW
// style set.
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // Retrieve the device context.
    CDC dc(lpDrawItemStruct->hDC);

    // Determine the button rectangle.
    CRect rect = lpDrawItemStruct->rcItem;

    // Draw in the button text.
    dc.DrawText(GetWindowText(), -1, rect, DT_CENTER | DT_VCENTER |
        DT_SINGLELINE);

    // Get the current state of the button.
    UINT state = lpDrawItemStruct->itemState;
    if ( (state & ODS_SELECTED) )   // If it is pressed.
        dc.DrawEdge(rect, EDGE_SUNKEN, BF_RECT); // Draw a sunken face.
    else
        dc.DrawEdge(rect, EDGE_RAISED, BF_RECT); // Draw a raised face.

    // Draw the focus rectangle, a dotted rectangle just inside the
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
}
