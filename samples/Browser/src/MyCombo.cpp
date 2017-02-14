//////////////////////////////////////////////
// ComboBoxEx.cpp

#include "stdafx.h"
#include "MyCombo.h"
#include "resource.h"


void CMyCombo::PreCreate(CREATESTRUCT& cs)
{
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}



