//////////////////////////////////////////////////////
// ImageView.h

#ifndef _IMAGEVIEW_H_
#define _IMAGEVIEW_H_


/////////////////////////////////////////////////////////////
// CImageView manages the window that displays the image. It
// inherits from CSrollView and display scroll bars when it's
// width or height is less than the image it displays.
class CImageView : public CScrollView
{
public:
    CImageView() = default;
    virtual ~CImageView() override = default;

    const CBitmap& GetImage() const { return m_image; }
    void     QuickPrint(LPCWSTR docName);
    void     Print(LPCWSTR docName);
    void     PrintPage(CDC& dc, int page = 1);
    bool     LoadFileImage(LPCWSTR filename);
    bool     SaveFileImage(LPCWSTR filename);

protected:
    virtual void    OnDraw(CDC& dc) override;
    virtual void    OnInitialUpdate() override;

private:
    CImageView(const CImageView&) = delete;
    CImageView& operator=(const CImageView&) = delete;

    CBitmap m_image;
};


#endif // _IMAGEVIEW_H_
