#ifndef TABCONTROL_H
#define TABCONTROL_H


class CTabCtrl : public CWnd
{
public:
	CTabCtrl() {}
	virtual ~CTabCtrl() {}
	virtual HWND Create(HWND hWndParent = NULL);

};

#endif

