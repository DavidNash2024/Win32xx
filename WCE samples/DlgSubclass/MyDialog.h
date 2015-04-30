///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "wxx_dialog.h"
#include "resource.h"
#include "Button.h"
#include "Hyperlink.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID);
	CMyDialog(LPCTSTR lpszResName);
	void SetStatic(LPCTSTR szString);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(UINT nID);
	virtual void OnOK();

private:
	CButton m_Button;
	CHyperlink m_Hyperlink;
};

#endif //MYDIALOG_H
