//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CViewMax and CMDIChildMax classes


#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H

#include "../../Win32++/mdi.h"

// Declaration of CViewMax
class CViewMax : public CWnd
{
	virtual void OnPaint(HDC hDC);
};

// Declaration of CMDIChildMax
class CMDIChildMax : public CMDIChild
{
public:
	CMDIChildMax();
	virtual ~CMDIChildMax();

protected:
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CViewMax m_MaxView;
};

#endif  //MDICHILDMAX_H
