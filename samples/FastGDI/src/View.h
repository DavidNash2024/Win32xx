//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


// Declaration of the CView class
class CView : public CScrollView
{
public:
    CView();
    virtual ~CView();
    virtual BOOL LoadFileImage(LPCTSTR filename);
    virtual BOOL SaveFileImage(LPCTSTR filename);
    CBitmap& GetImage() {return m_image;}
    virtual CRect GetImageRect();

protected:
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnInitialUpdate();
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CBitmap m_image;
};


#endif // VIEW_H
