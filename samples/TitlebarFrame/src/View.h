/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Doc.h"


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    CView() = default;
    virtual ~CView() override = default;
    CDoc& GetDoc();
    void PrintPage(CDC& dc, int page = 1);
    void QuickPrint(LPCWSTR docName);

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    CDoc m_doc;
};


#endif // VIEW_H
