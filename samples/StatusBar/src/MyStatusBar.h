#ifndef MYSTATUSBAR_H
#define MYSTATUSBAR_H

#include "Hyperlink.h"

class CMyStatusBar : public CStatusBar
{
public:
	CMyStatusBar() {}
	virtual ~CMyStatusBar() {}

protected:
	virtual void OnAttach();
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetStatusParts();

private:
	CHyperlink m_Hyperlink;
	CProgressBar m_ProgressBar;
};
















#endif  	// MYSTATUSBAR_H