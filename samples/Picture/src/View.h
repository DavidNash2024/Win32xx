/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include <olectl.h>


//////////////////////////////////////////
// CView manages CMainFrame's view window.
// It uses IPicture to render the image.
class CView : public CScrollView
{
public:
    CView();
    virtual ~CView() override;
    CRect GetImageRect();
    LPPICTURE GetPicture()  { return m_pPicture; }
    BOOL LoadPictureFile(LPCWSTR fileName);
    LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    void NewPictureFile();
    void SavePicture(LPCWSTR fileName);

protected:
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    LPPICTURE m_pPicture;
    CBrush  m_brush;
};


#endif // VIEW_H
