/////////////////////////////
// CoverImage.h
//

#ifndef _COVERIMAGE_H_
#define _COVERIMAGE_H_


////////////////////////////////////////////////////////////
// CCoverImage is used by the dialog to draw the cover image.
class CCoverImage : public CWnd
{
public:
    CCoverImage();
    virtual ~CCoverImage() override;
    CCoverImage(const CCoverImage&) = delete;
    CCoverImage& operator=(const CCoverImage&) = delete;

    std::vector<BYTE>& SetImageData() { return m_imageData; }

protected:
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    void DrawImage(CDC& dc);

    std::vector<BYTE> m_imageData;
    ULONG_PTR   m_gdiplusToken;
};

#endif // _COVERIMAGE_H_
