///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "..\Win32++\dialog.h"
#include "resource.h"
#include "button.h"
#include "Hyperlink.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	CMyDialog(LPCTSTR lpszResName, HWND hWndParent = NULL);
	void SetStatic(LPCTSTR szString);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	void OnButton();
	void OnRadio1();
	void OnRadio2();
	void OnRadio3();
	void OnCheck1();
	void OnCheck2();
	void OnCheck3();

	HMODULE m_hInstRichEdit;
	CButton m_Button;
	CHyperlink m_Hyperlink;
};

#endif //MYDIALOG_H
