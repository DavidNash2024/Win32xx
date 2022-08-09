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
    virtual ~CView();
    CRect GetImageRect();
    LPPICTURE GetPicture()  { return m_pPicture; }
    BOOL LoadPictureFile(LPCTSTR fileName);
    void NewPictureFile();
    void SavePicture(LPCTSTR fileName);

protected:
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator

    LPPICTURE m_pPicture;
    CBrush  m_brush;
};


#endif // VIEW_H
