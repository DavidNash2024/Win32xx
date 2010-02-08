//////////////////////////////////////////////
// ComboBoxEx.cpp

#include "stdafx.h"
#include "BrowserApp.h"
#include "ComboBoxEx.h"


void CComboBoxEx::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = _T("COMBOBOXEX32");
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}



