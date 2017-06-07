//////////////////////////////////////////////////////
// GDIPlusView.h

#ifndef GDIPLUSVIEW_H
#define GDIPLUSVIEW_H

#include <objidl.h>

#ifdef _MSC_VER
#pragma warning (disable : 4458) // disable warning: declaration hides class member
#endif
#include <gdiplus.h>
#ifdef _MSC_VER
#pragma warning (default : 4458) // return warning to default
#endif


// Declaration of the CGDIPlusView class
class CGDIPlusView : public CWnd
{
public:
	CGDIPlusView();
	virtual ~CGDIPlusView();

	void DrawCappedLine(CDC& dc);
	void DrawGamaShapes(CDC& dc);
	void DrawGradientElipse(CDC& dc);
	void DrawSolidElipse(CDC& dc);
	void DrawSolidLine(CDC& dc);
	void DrawText(CDC& dc);

protected:
	virtual void OnDraw(CDC& dc);
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual void RegisterClass(WNDCLASS& wc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	ULONG_PTR	m_gdiplusToken;
};


#endif // GDIPLUSVIEW_H
