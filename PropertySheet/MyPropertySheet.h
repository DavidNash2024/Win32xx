
#ifndef MYPROPERTYSHEET_H
#define MYPROPERTYSHEET_H

#include "../Win32++/PropertySheet.h"

INT_PTR CALLBACK  ButtonsDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK  ComboDlgProc(HWND, UINT, WPARAM, LPARAM);
int CALLBACK  PropSheetCallback(HWND, UINT, LPARAM);

void DoModalPropSheet(HWND hwndOwner);
HWND DoModelessPropSheet(HWND hwndOwner);
void DoWizardPropSheet(HWND hwndOwner);

class CMyPropertySheet : public CPropertySheet
{
public:
	CMyPropertySheet(LPCTSTR pszCaption = NULL, HWND hwndParent = NULL);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class CButtonPage : public CPropertyPage
{
public:
	CButtonPage(UINT nIDTemplate, LPCTSTR szTitle = NULL);
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnApply();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnQueryCancel();
	virtual void OnSetActive();
	virtual int Validate();
};

class CComboPage : public CPropertyPage
{
public:
	CComboPage(UINT nIDTemplate, LPCTSTR szTitle  = NULL);
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnSetActive();
};


#endif // MYPROPERTYSHEET_H

