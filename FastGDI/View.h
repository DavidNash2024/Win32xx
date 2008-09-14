//////////////////////////////////////////////////////
// View.h


#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/wincore.h"
#include "ColourDialog.h"


// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual void Tint();
	virtual BOOL FileOpen(LPCTSTR szFilename);

protected:
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HBITMAP m_hBitmap;
};


#endif // VIEW_H
