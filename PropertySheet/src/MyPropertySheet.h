//////////////////////////////////////////////
// MyPropertySheet.h

#ifndef MYPROPERTYSHEET_H
#define MYPROPERTYSHEET_H



class CMyPropertySheet : public CPropertySheet
{
public:
	CMyPropertySheet(LPCTSTR pszCaption = NULL, HWND hwndParent = NULL);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class CButtonPage : public CPropertyPage
{
public:
	CButtonPage(UINT nIDTemplate, LPCTSTR szTitle = NULL);
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int  OnApply();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual int  OnOK();
	virtual BOOL OnQueryCancel();
	virtual int  OnSetActive();
	virtual int  Validate();
};

class CComboPage : public CPropertyPage
{
public:
	CComboPage(UINT nIDTemplate, LPCTSTR szTitle  = NULL);
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual int  OnSetActive();
};


#endif // MYPROPERTYSHEET_H

