//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class


#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include "../Win32++/WinCore.h"

using std::vector;


class CView : public CWnd
{
public:
	CView();
	virtual ~CView(){}

protected:
	virtual void DrawLine(int x, int y);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	POINT m_OldPt;
};


#endif // CVIEW_H
