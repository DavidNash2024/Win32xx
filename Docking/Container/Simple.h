////////////////////////////////////////////
// Simple.h - Declaration of the CViewSimple 
//            and CDockSimple classes


#ifndef SIMPLE_H
#define SIMPLE_H

#include "../../Win32++/docking.h"


// Declaration of the CViewSimple class
class CViewSimple : public CWnd
{
public:
	CViewSimple() {}
	virtual ~CViewSimple() {}

protected:
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


// Declaration of the CDockSimple class
class CDockSimple : public CDocker
{
public:
	CDockSimple();
	virtual ~CDockSimple() {}
	virtual void OnInitialUpdate();

private:
	CViewSimple m_View;

};

#endif // SIMPLE_H
