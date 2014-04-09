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
	CEdit* GetEdit() const { return const_cast<CEdit*>(&m_Edit); } 

protected:
	virtual void OnAttach()
	{
		m_Edit.Attach(GetEditCtrl());
	}
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	CEdit m_Edit;
};


#endif
