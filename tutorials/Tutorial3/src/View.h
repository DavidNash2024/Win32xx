//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


#include "wxx_wincore.h"
#include <vector>


////////////////////////////////////////////////
// CView is the application's main window.
// In this view, we use the mouse to draw on the
// window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() override;

    void DrawLine(int x, int y);

protected:
    virtual void OnDestroy() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);

    POINT m_OldPt;
};


#endif // CVIEW_H
