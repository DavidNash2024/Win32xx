
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

	virtual BOOL OnApply()		{TRACE ("OnAppy"); return TRUE;}
	virtual void OnCancel()		{TRACE ("OnCancel"); }
	virtual BOOL OnInitDialog()	{TRACE ("OnInitDialog"); return TRUE;}
//	virtual BOOL OnKillActive()	{TRACE ("OnKillActive"); return TRUE;}
	virtual void OnOK()			{TRACE ("OnOK"); }
	virtual BOOL OnQueryCancel(){TRACE ("OnQueryCancel"); return TRUE;}
	virtual BOOL OnSetActive()	{TRACE ("Button OnSetActive"); PropSheet_SetWizButtons(GetParent(m_hWnd), PSWIZB_NEXT); return FALSE;}
	virtual LRESULT OnWizardBack()	{TRACE ("OnWizardBack"); return 0L;}
	virtual BOOL OnWizardFinish()	{TRACE ("OnWizardFinish"); return TRUE;}
	virtual LRESULT OnWizardNext()	{TRACE ("OnWizardNext"); return 0L;}

};

class CComboPage : public CPropertyPage
{
public:
	CComboPage(UINT nIDTemplate, LPCTSTR szTitle  = NULL);
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnSetActive()	{TRACE ("Combo OnSetActive"); PropSheet_SetWizButtons(GetParent(m_hWnd), PSWIZB_BACK | PSWIZB_FINISH); return FALSE;}
};


#endif // MYPROPERTYSHEET_H

