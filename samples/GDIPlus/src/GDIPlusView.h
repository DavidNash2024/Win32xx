//////////////////////////////////////////////////////
// GDIPlusView.h

#ifndef GDIPLUSVIEW_H
#define GDIPLUSVIEW_H

#include <objidl.h>
#include <gdiplus.h>
//#pragma comment (lib,"Gdiplus.lib")

// Declaration of the CGDIPlusView class
class CGDIPlusView : public CWnd
{
public:
	CGDIPlusView();
	virtual ~CGDIPlusView();

	void DrawCappedLine(HDC hdc);
	void DrawGamaShapes(HDC hdc);
	void DrawGradientElipse(HDC hdc);
	void DrawSolidElipse(HDC hdc);
	void DrawSolidLine(HDC hdc);
	void DrawText(HDC hdc);

protected:
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void RegisterClass(WNDCLASS &wc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	ULONG_PTR	m_gdiplusToken;
};


#endif // GDIPLUSVIEW_H
