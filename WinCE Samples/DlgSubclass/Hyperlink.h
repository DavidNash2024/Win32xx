/////////////////////////////
// Hyperlink.h

#ifndef HYPERLINK_H
#define HYPERLINK_H

#include "../WinDev++/WinCore.h"

#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

class CHyperlink :	public CWnd
{
public:
	CHyperlink();
	virtual ~CHyperlink();
	virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
	void OnLButtonDown();
	void OnLButtonUp(LPARAM lParam);

protected:
	virtual void OpenUrl();
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BOOL	m_bUrlVisited;
	BOOL	m_bClicked;
	COLORREF m_crVisited;
	COLORREF m_crNotVisited;
	HCURSOR m_hCursor;
	HFONT	m_hUrlFont;
};

#endif // HYPERLINK_H

