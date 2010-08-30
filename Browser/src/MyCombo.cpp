//////////////////////////////////////////////
// ComboBoxEx.cpp

#include "stdafx.h"
#include "BrowserApp.h"
#include "MyCombo.h"


void CMyCombo::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}



