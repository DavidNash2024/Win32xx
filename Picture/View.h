//////////////////////////////////////////////////////
// View.h


#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/WinCore.h"
#include <olectl.h>

// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual void LoadPictureFile(LPCTSTR szFile);

protected:
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LPOLESTR T2OLE(LPCTSTR szString);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HBRUSH m_hBrush;
	LPPICTURE m_pPicture;
	LPOLESTR m_OleString[MAX_STRING_SIZE];
};


#endif // VIEW_H
