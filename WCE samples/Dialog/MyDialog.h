#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "wxx_dialog.h"
#include "resource.h"


class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID);
	virtual ~CMyDialog();


protected:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

};

#endif //MYDIALOG_H
