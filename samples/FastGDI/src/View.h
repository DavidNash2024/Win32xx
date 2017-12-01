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
    virtual BOOL LoadFileImage(LPCTSTR szFilename);
    virtual BOOL SaveFileImage(LPCTSTR pszFile);
    CBitmap& GetImage() {return m_bmImage;}
    virtual CRect GetImageRect();

protected:
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void    OnInitialUpdate();
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CBitmap m_bmImage;
};


#endif // VIEW_H
