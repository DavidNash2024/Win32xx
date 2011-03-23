//////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H


#include "resource.h"

class CMyCombo : public CComboBoxEx
{
public:
	CMyCombo() {}
	virtual ~CMyCombo() {}
	virtual void PreCreate(CREATESTRUCT &cs);
};


#endif
