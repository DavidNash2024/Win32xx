//////////////////////////////////////////////////
// Simple.h - Declaration of the CViewSimple class


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


#endif // SIMPLE_H
