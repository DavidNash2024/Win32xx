///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "dialog.h"
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
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(UINT nID);
	virtual void OnOK();

private:
	CButton m_Button;
	CHyperlink m_Hyperlink;
};

#endif //MYDIALOG_H
