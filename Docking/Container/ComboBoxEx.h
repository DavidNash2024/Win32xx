//////////////////////////////////////////////////////
// ComboBoxEx.h


#ifndef COMBOBOXEX_H
#define COMBOBOXEX_H

#include "../../Win32++/wincore.h"


// Declaration of the CComboBoxEx class
class CComboBoxEx : public CWnd
{
public:
	CComboBoxEx();
	virtual ~CComboBoxEx();
	BOOL AddItems();
	void SetImages(int nImages, UINT ImageID);

protected:
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	HIMAGELIST m_himlImages;
};


#endif // COMBOBOXEX_H
