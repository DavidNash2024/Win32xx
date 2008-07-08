///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "../DevWin++/dialog.h"
#include "resource.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CMyDialog();

	void SetProgress(int nPos);
	void SetScroll(int nPos);
	void SetStatic(BOOL IsSlider, int nPos);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnHScroll(WPARAM wParam, LPARAM lParam);
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	SCROLLINFO m_si;
};

#endif //MYDIALOG_H
