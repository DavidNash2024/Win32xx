///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "../DevWin++/dialog.h"
#include "Progressbar.h"
#include "Scrollbar.h"
#include "Slider.h"
#include "resource.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CMyDialog();

	void SetProgress(int nPos);
	void SetScroll(int nPos);
	void SetSlider(int nPos);
	void SetStatic(BOOL IsSlider, int nPos);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CProgressbar m_Progressbar;
	CScrollbar m_Scrollbar;
	CSlider m_Slider;
};

#endif //MYDIALOG_H
