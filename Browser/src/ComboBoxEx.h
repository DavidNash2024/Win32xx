#ifndef _COMBOBOXEX_H_
#define _COMBOBOXEX_H_

#include "../../Win32++/wincore.h"
#include "resource.h"

class CComboBoxEx : public CWnd
{
public:
	CComboBoxEx() {}
	virtual ~CComboBoxEx() {}
	virtual void PreCreate(CREATESTRUCT &cs);
};

class CComboEdit : public CWnd
{
public:
	CComboEdit() {}
	virtual ~CComboEdit() {}
};


#endif