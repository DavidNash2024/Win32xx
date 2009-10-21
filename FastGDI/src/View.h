//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include "ColourDialog.h"


// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual BOOL FileOpen(LPCTSTR szFilename);
	virtual BOOL FileSave(LPCTSTR pszFile);
	HBITMAP GetImage() {return m_hbmImage;}
	virtual CRect GetImageRect();

protected:
	virtual void OnHScroll(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void OnVScroll(WPARAM wParam, LPARAM lParam);
	virtual void OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HBITMAP m_hbmImage;
	HBITMAP m_hbmSmallImage;
	int m_xCurrentScroll;
	int m_yCurrentScroll;
};


#endif // VIEW_H
