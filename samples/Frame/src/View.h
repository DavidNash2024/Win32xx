//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include "Doc.h"

// Declaration of the CView class
class CView : public CWnd
{
public:
    CView();
    virtual ~CView(){}
    CDoc& GetDoc();
    void Print(LPCTSTR docName);
    void PrintPage(CDC& dc, UINT page = 1);
    void QuickPrint(LPCTSTR docName);
    void SaveViewImage();

protected:
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDoc m_doc;
    CBitmap m_viewImage;
};


#endif // VIEW_H
