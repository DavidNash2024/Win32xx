/////////////////////////////
// ComboBoxEx.cpp
//

#include "stdafx.h"
#include "MyCombo.h"
#include "resource.h"

////////////////////////////////
// CMyCombo function definitions
//

// Called when the window handle (HWND) is attached to CMyCombo.
void CMyCombo::OnAttach()
{
    m_edit.Attach(GetEditCtrl());
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMyCombo::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;
    cs.cy = 100;    // required to display list
    cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}



