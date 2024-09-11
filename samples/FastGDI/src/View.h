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
    virtual ~CView() override;

    CBitmap& GetImage() { return m_image; }
    CRect    GetImageRect();
    void     QuickPrint(LPCWSTR docName);
    void     Print(LPCWSTR docName);
    void     PrintPage(CDC& dc, int page = 1);
    BOOL     LoadFileImage(LPCWSTR filename);
    BOOL     SaveFileImage(LPCWSTR filename);

protected:
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc) override;
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnInitialUpdate() override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    CBitmap m_image;
};


#endif // VIEW_H
