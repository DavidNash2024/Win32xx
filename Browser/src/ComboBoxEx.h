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
};


#endif