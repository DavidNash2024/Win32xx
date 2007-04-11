///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "..\Win32++\dialog.h"
#include "resource.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

};

#endif //MYDIALOG_H
