#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "dialog.h"
#include "resource.h"


class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CMyDialog();


protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

};

#endif //MYDIALOG_H
