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
    BOOL     LoadFileImage(LPCTSTR filename);
    BOOL     SaveFileImage(LPCTSTR filename);
    CBitmap& GetImage() {return m_image;}
    CRect    GetImageRect();
    void     QuickPrint(LPCTSTR docName);
    void     Print(LPCTSTR docName);
    void     PrintPage(CDC& dc, UINT page = 1);

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
