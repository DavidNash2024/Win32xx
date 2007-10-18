#ifndef PROPERTYSHEET_H
#define PROPERTYSHEET_H

#include "../Win32++/Dialog.h"

#define ID_APPLY_NOW 0x3021

namespace Win32xx
{

	class CPropertyPage : public CWnd
	{
	public:
		CPropertyPage (UINT nIDTemplate, LPCTSTR szTitle = NULL);
		virtual ~CPropertyPage() {}

		virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL DialogProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		PROPSHEETPAGE GetPSP() {return m_PSP;}
		virtual void OnApply() { TRACE("Apply");}
		virtual void OnCancel() {TRACE("Cancel");}
		virtual BOOL OnInitDialog() { return TRUE;}
		virtual void OnOK() {TRACE("OK");}
		virtual void OnSetActive() {}
		virtual void OnReset() {TRACE("Reset");}
		virtual void OnWizardBack() {}
		virtual void OnWizardFinish() {}
		virtual void OnWizardNext() {}
		static BOOL CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		PROPSHEETPAGE m_PSP;

	private:
		TCHAR m_szTitle[MAX_STRING_SIZE];
	};

	class CPropertySheet : public CWnd
	{
	public:
		CPropertySheet(UINT nIDCaption, HWND hwndParent = NULL);
		CPropertySheet(LPCTSTR pszCaption = NULL, HWND hwndParent = NULL);
		virtual ~CPropertySheet(); 

		void AddPage(CPropertyPage* pPage);
		virtual HWND Create(HWND hWndParent = 0);
		INT_PTR CreatePropertySheet(LPCPROPSHEETHEADER ppsph);
		virtual void DestroyWindow();
		int DoModal();
		void GetActiveIndex() {}
		void GetActivePage() {}
		void GetPage() {}
		int GetPageCount() {return (int)m_vPages.size();}
		void GetPageIndex() {}
		void GetTabControl() {}
		virtual void OnInitSheet() {}
		void RemovePage(CPropertyPage* pPage) {}
		void SetActivePage() {}
		void SetTitle() {}
		void SetWizardMode(BOOL bWizard);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void CALLBACK Callback(HWND hwnd, UINT uMsg, LPARAM lParam);

	protected:
		PROPSHEETHEADER m_PSH;
		std::vector<CPropertyPage*> m_vPages;

	private:
		TCHAR m_szCaption[MAX_STRING_SIZE];
	
	};

}

#endif // PROPERTYSHEET_H
