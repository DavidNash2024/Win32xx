//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


//////////////////////////////////////////////////////
// CView manages CMainFrame's view window. It displays
// the image.
class CView : public CScrollView
{
public:
    CView();
    virtual ~CView();

    CBitmap& GetImage() { return m_image; }
    CRect    GetImageRect();
    void     QuickPrint(LPCTSTR docName);
    void     Print(LPCTSTR docName);
    void     PrintPage(CDC& dc, UINT page = 1);
    BOOL     LoadFileImage(LPCTSTR filename);
    BOOL     SaveFileImage(LPCTSTR filename);

protected:
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnInitialUpdate();
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CBitmap m_image;
};


#endif // VIEW_H
