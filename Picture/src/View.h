//////////////////////////////////////////////////////
// View.h


#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/wincore.h"
#include <olectl.h>

// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual RECT GetImageRect();
	virtual void LoadPictureFile(LPCTSTR szFile);
	virtual void SavePicture(LPCTSTR szFile);

	LPPICTURE m_pPicture;

protected:
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LPOLESTR T2OLE(LPCTSTR szString);
	virtual BSTR T2BSTR(LPCTSTR szString);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HBRUSH m_hBrush;
	OLECHAR m_OleString[MAX_STRING_SIZE];
	BSTR    m_BStrString;
};


#endif // VIEW_H
