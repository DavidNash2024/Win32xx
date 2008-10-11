//////////////////////////////////////////////////////
// MDIChildRect.h
//  Declaration of the CRectView and CRectMDIChild classes


#ifndef MDICHILDRECT_H
#define MDICHILDRECT_H

#include "../Win32++/mdi.h"

// Declaration of CRectView
class CRectView : public CWnd
{
public:
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};


// Declaration of CRectMDIChild
class CRectMDIChild : public CMDIChild
{
public:
	CRectMDIChild();
	virtual ~CRectMDIChild();

protected:
	virtual void OnInitialUpdate();

private:
	CRectView m_RectView;

};


#endif  //MDICHILDRECT_H
