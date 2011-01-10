///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "resource.h"

// Declaration of the CButtonDialog class
class CButtonDialog : public CDialog
{
public:
	CButtonDialog(UINT nResID, CWnd* pParent = NULL);
	~CButtonDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	HBRUSH m_hBrush;
};

class CComboBoxDialog : public CDialog
{
public:
	CComboBoxDialog	(UINT nResID, CWnd* pParent = NULL);
	~CComboBoxDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();

private:
	HBRUSH m_hBrush;
};


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, CWnd* pParent = NULL);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	CTab m_Tab;
};

#endif //MYDIALOG_H
