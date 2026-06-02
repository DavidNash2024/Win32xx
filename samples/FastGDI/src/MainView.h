//////////////////////////////////////////////////////
// MainView.h

#ifndef _MAINVIEW_H_
#define _MAINVIEW_H_

#include "ImageView.h"


///////////////////////////////////////////////////////////
// CMainView manages the the view window for CMainFrame.
// It displays the image managed by CImageView. The image
// managed by CImageView is centered within the main view.
class CMainView : public CWnd
{
public:
    CMainView() = default;
    virtual ~CMainView() override = default;

    const CBitmap& GetImage() const { return m_imageView.GetImage(); }
    void RecalcLayout();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDraw(CDC& dc) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainView(const CMainView&) = delete;
    CMainView& operator=(const CMainView&) = delete;

    LRESULT OnDropFiles(UINT, WPARAM, LPARAM);
    LRESULT OnSize(UINT, WPARAM, LPARAM);

public:
    CImageView m_imageView;
};

#endif // _MAINVIEW_H_
