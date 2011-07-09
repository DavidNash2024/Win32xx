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
	HBITMAP GetImage() {return m_bmImage;}
	virtual CRect GetImageRect();

protected:
	virtual void OnHScroll(WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* pDC);	
	virtual void OnInitialUpdate();
	virtual void OnVScroll(WPARAM wParam, LPARAM lParam);
	virtual void OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CBitmap m_bmImage;
	int m_xCurrentScroll;
	int m_yCurrentScroll;
};


#endif // VIEW_H
