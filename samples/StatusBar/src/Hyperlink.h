/////////////////////////////
// Hyperlink.h

#ifndef HYPERLINK_H
#define HYPERLINK_H


#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

// Creates a window that behaves like a hyperlink.
// * A hand cursor is displayed when the Mouse hovers over the text
// * A browser is launched and the Win32++ SourceForge page is displayed
//    when the text is clicked.
// * The text color is adjusted.

class CHyperlink :	public CStatic
{
public:
	CHyperlink();
	virtual ~CHyperlink();
	virtual void OnAttach();
	void OnLButtonDown();
	void OnLButtonUp(LPARAM lParam);
	LRESULT OnSetCursor();

protected:
	virtual void OpenUrl();
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BOOL	m_bUrlVisited;
	BOOL	m_bClicked;
	COLORREF m_crVisited;
	COLORREF m_crNotVisited;
	HCURSOR m_hCursor;
	CFont	m_UrlFont;
};

#endif // HYPERLINK_H

