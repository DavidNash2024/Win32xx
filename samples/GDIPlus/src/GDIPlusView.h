/////////////////////////////
// GDIPlusView.h
//

#ifndef GDIPLUSVIEW_H
#define GDIPLUSVIEW_H

#include <objidl.h>


#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (disable : 4458) // disable warning: declaration hides class member
#endif

// Declare min and max for older versions of Visual Studio
#if defined (_MSC_VER) && (_MSC_VER < 1920) // < VS2019
using std::min;
using std::max;
#endif

#include <gdiplus.h>

#if defined(_MSC_VER) && (_MSC_VER == 1900)
#pragma warning (default : 4458) // return warning to default
#endif


//////////////////////////////////////////////////
// CView manages CMainFrame's view window. It uses
// GDIPlus to perform drawing.
class CGDIPlusView : public CWnd
{
public:
    CGDIPlusView();
    virtual ~CGDIPlusView() override;

    void DrawCappedLine(CDC& dc);
    void DrawGamaShapes(CDC& dc);
    void DrawGradientElipse(CDC& dc);
    void DrawSolidElipse(CDC& dc);
    void DrawSolidLine(CDC& dc);
    void DrawText(CDC& dc);
    void RegisterClass(WNDCLASS& wc);

protected:
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CGDIPlusView(const CGDIPlusView&) = delete;
    CGDIPlusView& operator=(const CGDIPlusView&) = delete;

    ULONG_PTR   m_gdiplusToken;
};


#endif // GDIPLUSVIEW_H
