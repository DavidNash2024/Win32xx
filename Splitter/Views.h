/////////////////////////////////////////////////
// Views.h
//  Declaration of the CTopView, CBottomView
//   and CView classes


#ifndef VIEWS_H
#define VIEWS_H

#include <tchar.h>
#include "../Win32++/Splitter.h"


class CView : public CWnd
{
public:
	CView();
	~CView(){}
	virtual TCHAR* GetString(){return m_str;};
	virtual void SetString(TCHAR* str);

protected:
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	TCHAR m_str[41];
};

class CTopView : public CSplitter
{
public:
	CTopView();
	virtual ~CTopView(){}

private:
	CView m_TopLeft;
	CView m_TopRight;
};

class CBottomView : public CSplitter
{
public:
	CBottomView();
	virtual ~CBottomView(){}

private:
	CView m_BottomLeft;
	CView m_BottomRight;
};

class CMainView : public CSplitter
{
public:
	CMainView(int nBarPos = 0, bool bVertical = true);
	virtual ~CMainView(){}
	virtual void Reposition();

protected:
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTopView m_Top;
	CBottomView m_Bottom;
};


#endif // CVIEWS_H
