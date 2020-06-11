#ifndef _COVERIMAGE_H_
#define _COVERIMAGE_H_

// A class used by the dialog to draw the cover image.
class CCoverImage : public CWnd
{
public:
    CCoverImage();
    virtual ~CCoverImage();

    void DrawImage(CDC& dc);
    std::vector<BYTE>& GetImageData() {return m_imageData;}

protected:
    void OnDraw(CDC& dc);
    LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    std::vector<BYTE> m_imageData;
    ULONG_PTR   m_gdiplusToken;
};

#endif // _COVERIMAGE_H_
