///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "../Win32++/dialog.h"
#include "resource.h"

// Declaration of the CButtonDialog class
class CButtonDialog : public CDialog
{
public:
	CButtonDialog(UINT nResID, HWND hWnd = NULL) : CDialog(nResID, hWnd) {}

};

class CComboBoxDialog : public CDialog
{
public:
	CComboBoxDialog	(UINT nResID, HWND hWnd = NULL) : CDialog(nResID, hWnd)  {}

};


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);

	void ShowButtonDialog();
	void ShowComboBoxDialog();

private:
	CButtonDialog m_ButtonDialog;
	CComboBoxDialog m_ComboBoxDialog;
};

#endif //MYDIALOG_H
