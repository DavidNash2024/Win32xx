/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include <olectl.h>

/////////////////////////////////
// Declaration of the CView class
//
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
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    LPPICTURE m_pPicture;
    CBrush  m_brush;
};


#endif // VIEW_H
