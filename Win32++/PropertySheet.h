// Win32++  Version 5.7
// Released: 15th February, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// PropertySheet.h
//  Declaration of the following classes:
//  CPropertyPage and CPropertySheet

#ifndef PROPERTYSHEET_H
#define PROPERTYSHEET_H

#include "../Win32++/Dialog.h"

#define ID_APPLY_NOW   0x3021
#define ID_WIZBACK     0x3023
#define ID_WIZNEXT     0x3024
#define ID_WIZFINISH   0x3025
#define ID_HELP        0xE146

#ifndef PROPSHEETHEADER_V1_SIZE
 #define PROPSHEETHEADER_V1_SIZE 40
#endif

namespace Win32xx
{

	class CPropertyPage : public CWnd
	{
	public:
		CPropertyPage (UINT nIDTemplate, LPCTSTR szTitle = NULL);
		virtual ~CPropertyPage() {}
		virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL DialogProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual int Validate();
		virtual void OnApply();
		virtual void OnCancel();
		virtual BOOL OnInitDialog();
		virtual void OnKillActive();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnOK();
		virtual BOOL OnQueryCancel();
		virtual void OnSetActive();
		virtual void OnWizardBack();
		virtual BOOL OnWizardFinish();
		virtual void OnWizardNext();
		virtual LRESULT QuerySiblings(WPARAM wParam, LPARAM lParam);

		static UINT CALLBACK StaticPropSheetPageProc(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE ppsp);
		static BOOL CALLBACK StaticDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void CancelToClose() const;
		PROPSHEETPAGE GetPSP() const {return m_PSP;}
		BOOL IsButtonEnabled(int iButton) const;
		void SetModified(BOOL bChanged) const;

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
		virtual void AddPage(CPropertyPage* pPage);
		virtual HWND Create(HWND hWndParent = 0);
		virtual INT_PTR CreatePropertySheet(LPCPROPSHEETHEADER ppsph);
		virtual void CPropertySheet::DestroyButton(int iButton);
		virtual void DestroyWindow();
		virtual int DoModal();
		virtual void OnCreate();
		virtual void OnInitialUpdate();
		virtual void RemovePage(CPropertyPage* pPage);
		virtual BOOL SetActivePage(int nPage);
		virtual BOOL SetActivePage(CPropertyPage* pPage);
		virtual void SetTitle(LPCTSTR szTitle);
		virtual void SetWizardMode(BOOL bWizard);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static void CALLBACK Callback(HWND hwnd, UINT uMsg, LPARAM lParam);

		CPropertyPage* GetActivePage() const;
		int GetPageCount() const;
		int GetPageIndex(CPropertyPage* pPage) const;
		HWND GetTabControl()const;
		BOOL IsModeless() const;
		BOOL IsWizard() const;

	protected:
		PROPSHEETHEADER m_PSH;
		std::vector<CPropertyPage*> m_vPages;

	private:
		void BuildPageArray();

		TCHAR m_szCaption[MAX_STRING_SIZE];
		PROPSHEETPAGE* m_ppsp; // Array of PROPSHEETPAGE

	};

}

#endif // PROPERTYSHEET_H
