#ifndef PROPERTYSHEET_H
#define PROPERTYSHEET_H

#include "../Win32++/Dialog.h"

#define ID_APPLY_NOW   0x3021
#define ID_WIZBACK     0x3023
#define ID_WIZNEXT     0x3024
#define ID_WIZFINISH   0x3025
#define ID_HELP        0xE146

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
		BOOL IsButtonEnabled(int iButton);
		virtual int Validate();
		virtual BOOL OnApply();
		virtual void OnCancel();
		virtual BOOL OnInitDialog();
		virtual BOOL OnKillActive();
		virtual void OnOK();
		virtual BOOL OnQueryCancel();
		virtual BOOL OnSetActive();
		virtual LRESULT OnWizardBack();
		virtual BOOL OnWizardFinish();
		virtual LRESULT OnWizardNext();
		void CancelToClose() {/*Not implemented*/}
		void QuerySiblings() {/*Not implemented*/}
		void SetModified()   {/*Not implemented*/}

		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);

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
		void BuildPageArray();
		virtual HWND Create(HWND hWndParent = 0);
		INT_PTR CreatePropertySheet(LPCPROPSHEETHEADER ppsph);
		virtual void DestroyWindow();
		int DoModal();
		CPropertyPage* GetActivePage();
		int GetPageCount();
		int GetPageIndex(CPropertyPage* pPage);
		HWND GetTabControl();
		void RemovePage(CPropertyPage* pPage);
		BOOL SetActivePage(int nPage);
		BOOL SetActivePage(CPropertyPage* pPage);
		void SetTitle(LPCTSTR szTitle);
		void SetWizardMode(BOOL bWizard);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void CALLBACK Callback(HWND hwnd, UINT uMsg, LPARAM lParam);

	protected:
		PROPSHEETHEADER m_PSH;
		std::vector<CPropertyPage*> m_vPages;

	private:
		TCHAR m_szCaption[MAX_STRING_SIZE];
		PROPSHEETPAGE* m_ppsp; // Array of PROPSHEETPAGE
	
	};

}

#endif // PROPERTYSHEET_H
