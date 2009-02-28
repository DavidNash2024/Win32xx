//////////////////////////////////////////////////////
// Rect.h
//  Declaration of the CViewRect class


#ifndef RECT_H
#define RECT_H

#include "../../Win32++/wincore.h"

// Declaration of CViewRect
class CViewRect : public CWnd
{
public:
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};




#endif  //RECT_H
