//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CMaxView and CMaxMDIChild classes


#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H

#include "../Win32++/mdi.h"

// Declaration of CMaxView
class CMaxView : public CWnd
{
	virtual void OnPaint(HDC hDC);
};

// Declaration of CMaxMDIChild
class CMaxMDIChild : public CMDIChild
{
public:
	CMaxMDIChild();
	virtual ~CMaxMDIChild();

protected:
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CMaxView m_MaxView;
};

#endif  //MDICHILDMAX_H
