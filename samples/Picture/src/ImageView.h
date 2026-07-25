/////////////////////////////
// ImageView.h
//

#ifndef IMAGEVIEW_H_
#define IMAGEVIEW_H_

#include <olectl.h>


/////////////////////////////////////////////////////////
// CImageView manages the window that displays the image.
// It uses IPicture to render the image.
class CImageView : public CScrollView
{
public:
    CImageView();
    virtual ~CImageView() override;
    CSize GetImageSize() const;
    LPPICTURE GetPicture() const { return m_pPicture; }
    bool LoadPictureFile(LPCWSTR fileName);
    void NewPictureFile();
    void SavePicture(LPCWSTR fileName);

protected:
    virtual void    OnDraw(CDC& dc) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;

private:
    CImageView(const CImageView&) = delete;
    CImageView& operator=(const CImageView&) = delete;

    LPPICTURE m_pPicture;
    CBrush  m_brush;
};


#endif // IMAGEVIEW_H_
