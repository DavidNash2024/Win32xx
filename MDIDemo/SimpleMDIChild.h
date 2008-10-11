//////////////////////////////////////////////////////
// MDIChildView.h
//  Declaration of the CSimpleView and CSimpleMDIChild class


#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H

#include "../Win32++/mdi.h"

// Declaration of CSimpleView
class CSimpleView : public CWnd
{
public:
	CSimpleView();
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	COLORREF GetColor() {return m_Color;}
	void SetColor(COLORREF color) { m_Color = color; }

private:
	COLORREF m_Color;
};


// Declaration of CSimpleMDIChild
class CSimpleMDIChild : public CMDIChild
{
public:
	CSimpleMDIChild();
	virtual ~CSimpleMDIChild();

protected:
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CSimpleView m_View;
};

#endif  //MDICHILDVIEW_H
