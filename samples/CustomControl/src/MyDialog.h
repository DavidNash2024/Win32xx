///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyWebControl.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, CWnd* pParent = NULL);
	virtual ~CMyDialog();

protected:
	virtual void OnDestroy();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CResizer m_Resizer;
	CWebControl m_MyControl;
};

#endif //MYDIALOG_H
