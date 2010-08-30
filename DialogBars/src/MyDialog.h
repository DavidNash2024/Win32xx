///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


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
	CMyProgressbar m_Progressbar;
	CMyScrollbar m_Scrollbar;
	CMySlider m_Slider;
};

#endif //MYDIALOG_H
