#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "..\Win32++\dialog.h"
#include "resource.h"


class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CMyDialog();
	void AddToEdit();
	void AddToListBox();
	void AddToProgressBar();
	void AddToScrollBars();
	void AddToSlider();
	void AddToComboBox();
	void AddToButton();

private:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

	int m_nCounter;
};

#endif //MYDIALOG_H
