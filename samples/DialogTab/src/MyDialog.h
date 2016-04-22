///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "resource.h"

// Declaration of the CButtonDialog class
class CButtonDialog : public CDialog
{
public:
	CButtonDialog(UINT nResID);
	~CButtonDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual INT_PTR OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual INT_PTR OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnOK() {}		// supress closing dialog with the enter key
	virtual void OnCancel() {}	// supress closing dialog with the esc key

private:
	CBrush m_Brush;
};

class CComboBoxDialog : public CDialog
{
public:
	CComboBoxDialog	(UINT nResID);
	~CComboBoxDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK() {}		// supress closing dialog with the enter key
	virtual void OnCancel() {}	// supress closing dialog with the esc key

private:
	CBrush m_Brush;
};


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID);
	virtual ~CMyDialog();

protected:
	virtual void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	CTab m_Tab;
};

#endif //MYDIALOG_H
