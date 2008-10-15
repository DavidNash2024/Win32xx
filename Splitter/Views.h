/////////////////////////////////////////////////
// Views.h
//  Declaration of the CMainPane, CBottomPane
//   and CView classes


#ifndef VIEWS_H
#define VIEWS_H

#include <tchar.h>
#include "../Win32++/dockable.h"
#include "../Win32++/listview.h"
#include "../Win32++/treeview.h"


class CMySimpleView : public CWnd
{
protected:
	virtual void OnPaint(HDC hDC);
};

class CMyTreeView : public CTreeView
{
};

class CMyTextView : public CWnd
{
protected:
	virtual void PreCreate(CREATESTRUCT &cs) 
	{
		cs.lpszClass = _T("EDIT");
		cs.style = ES_MULTILINE|WS_CHILD;
	}
	virtual void OnInitialUpdate() {SetWindowText(_T("Text View"));}
};

class CMyListView : public CListView
{
};


class CMainPane : public CDockable
{
public:
	CMainPane() {SetView(m_View);}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
	CMySimpleView m_View;
};

class CTopRightPane : public CDockable
{
public:
	CTopRightPane() {SetView(m_View);}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
//	CMyTextView m_View;
	CMySimpleView m_View;
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
//	CMyTreeView m_View;
	CMySimpleView m_View;
};

class CBottomRightPane : public CDockable
{
public:
	CBottomRightPane() {SetView(m_View);}
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

private:
//	CMyListView m_View;
	CMySimpleView m_View;
};


#endif // VIEWS_H
