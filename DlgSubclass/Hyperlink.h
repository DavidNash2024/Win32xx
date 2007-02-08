/////////////////////////////
// Hyperlink.h

#ifndef HYPERLINK_H
#define HYPERLINK_H

#include "..\Win32++\WinCore.h"

class CHyperlink :	public CWnd
{
public:
	CHyperlink();
	virtual ~CHyperlink();
	virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
	void OnLButtonDown();
	void OnLButtonUp(LPARAM lParam);

protected:
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void DrawUrl(HDC hDC);
	virtual void OpenUrl();

private:

	BOOL	m_bUrlVisited;
	COLORREF m_crVisited, m_crNotVisited;
	HFONT	m_hUrlFont;		// underlined font
	BOOL	m_bClicked;		// true when the url link is clicked
	HCURSOR m_hCursor;		// Show special cursor when using the URL
};

#endif // HYPERLINK_H

