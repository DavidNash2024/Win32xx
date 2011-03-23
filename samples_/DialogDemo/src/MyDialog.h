///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyButton.h"
#include "Hyperlink.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, CWnd* pParent = NULL);
	CMyDialog(LPCTSTR lpszResName, CWnd* pParent = NULL);
	void SetStatic(LPCTSTR szString);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
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
	CMyButton m_Button;
	CHyperlink m_Hyperlink;
};

#endif //MYDIALOG_H
