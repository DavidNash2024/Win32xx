//////////////////////////////////////////////////////
// MDIChildSimple.h
//  Declaration of the CViewSimple and CMDIChildSimple class


#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H

#include "../../Win32++/mdi.h"

// Declaration of CViewSimple
class CViewSimple : public CWnd
{
public:
	CViewSimple();
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	COLORREF GetColor() {return m_Color;}
	void SetColor(COLORREF color) { m_Color = color; }

private:
	COLORREF m_Color;
};


// Declaration of CMDIChildSimple
class CMDIChildSimple : public CMDIChild
{
public:
	CMDIChildSimple();
	virtual ~CMDIChildSimple();

protected:
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CViewSimple m_View;
};

#endif  //MDICHILDVIEW_H
