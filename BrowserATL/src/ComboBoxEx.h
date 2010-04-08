//////////////////////////////////////////////
// ComboBoxEx.h

#ifndef COMBOBOXEX_H
#define COMBOBOXEX_H


#include "resource.h"

class CComboBoxEx : public CWnd
{
public:
	CComboBoxEx() {}
	virtual ~CComboBoxEx() {}
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

};

class CComboEdit : public CWnd
{
public:
	CComboEdit() {}
	virtual ~CComboEdit() {}

};


#endif