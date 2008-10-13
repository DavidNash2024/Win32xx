/////////////////////////////////////////////////
// Views.h
//  Declaration of the CMainPane, CBottomPane
//   and CView classes


#ifndef VIEWS_H
#define VIEWS_H

#include <tchar.h>
#include "../Win32++/dockable.h"


class CTopLeftView : public CWnd
{
protected:
	virtual void OnPaint(HDC hDC);
};

class CTopRightView : public CWnd
{
protected:
	virtual void OnPaint(HDC hDC);
};

class CBottomLeftView : public CWnd
{
protected:
	virtual void PreCreate(CREATESTRUCT &cs) 
	{
		cs.lpszClass = _T("EDIT");
	}
};

class CBottomRightView : public CWnd
{
protected:
	virtual void PreCreate(CREATESTRUCT &cs) 
	{
		cs.lpszClass = _T("EDIT");
	}
};


class CMainPane : public CDockable
{
public:
	CMainPane() {SetView(m_View);}
//	virtual void OnInitialUpdate() {SetBarWidth(10);}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
	CTopLeftView m_View;
};

class CTopRightPane : public CDockable
{
public:
	CTopRightPane() {SetView(m_View);}
//	virtual void OnInitialUpdate() {SetBarWidth(10);}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
	CTopRightView m_View;
};

class CBottomPane : public CDockable
{
public:
	CBottomPane() {SetView(m_View);}
	virtual void OnInitialUpdate() 
	{
		SetBarColor(RGB(255, 0, 0));
		SetBarWidth(10);
	}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
	CBottomLeftView m_View;
};

class CBottomRightPane : public CDockable
{
public:
	CBottomRightPane() {SetView(m_View);}
//	virtual void OnInitialUpdate() {SetBarWidth(10);}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
	CBottomRightView m_View;
};


#endif // VIEWS_H
