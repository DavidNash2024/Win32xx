
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

	virtual void OnApply()		{ TRACE ("OnAppy"); }
	virtual void OnCancel()		{ TRACE ("OnCancel"); }
	virtual BOOL OnInitDialog()	{ TRACE ("OnInitDialog"); return TRUE; }
//	virtual BOOL OnKillActive()	{ TRACE ("OnKillActive"); }
	virtual void OnOK()			{ TRACE ("OnOK"); }
	virtual BOOL OnQueryCancel(){ TRACE ("OnQueryCancel"); return TRUE; }
	virtual void OnSetActive()	{ TRACE ("Button OnSetActive"); PropSheet_SetWizButtons(GetParent(m_hWnd), PSWIZB_NEXT); }
	virtual void OnWizardBack()	{ TRACE ("OnWizardBack"); }
	virtual BOOL OnWizardFinish()	{ TRACE ("OnWizardFinish"); return TRUE; }
	virtual void OnWizardNext()	{ TRACE ("OnWizardNext"); }

};

class CComboPage : public CPropertyPage
{
public:
	CComboPage(UINT nIDTemplate, LPCTSTR szTitle  = NULL);
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnSetActive()	{ TRACE ("Combo OnSetActive"); PropSheet_SetWizButtons(GetParent(m_hWnd), PSWIZB_BACK | PSWIZB_FINISH); }
};


#endif // MYPROPERTYSHEET_H

