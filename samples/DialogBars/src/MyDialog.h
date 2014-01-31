///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "ProgressBar.h"
#include "ScrollBar.h"
#include "Slider.h"
#include "resource.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID, CWnd* pParent = NULL);
	virtual ~CMyDialog();

	void SetProgress(int nPos);
	void SetScroll(int nPos);
	void SetSlider(int nPos);
	void SetStatic(BOOL IsSlider, int nPos);

protected:
	virtual void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	CMyProgressBar m_ProgressBar;
	CMyScrollBar m_ScrollBar;
	CMySlider m_Slider;
};

#endif //MYDIALOG_H
