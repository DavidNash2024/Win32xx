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
    virtual ~CCoverImage();
    std::vector<BYTE>& SetImageData() { return m_imageData; }

protected:
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void DrawImage(CDC& dc); 

    std::vector<BYTE> m_imageData;
    ULONG_PTR   m_gdiplusToken;
};

#endif // _COVERIMAGE_H_
