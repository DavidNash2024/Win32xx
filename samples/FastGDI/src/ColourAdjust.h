////////////////////////
// ColourAdjust.h

#ifndef _COLOURADJUST_H_
#define _COLOURADJUST_H_


/////////////////////////////////////////////////////
// CColorAdjust manages a dialog that allows the
// user to choose the color adjustment for the image.
class CColorAdjust : public CDialog
{
public:
    CColorAdjust(UINT resID, const CBitmap& image);
    virtual ~CColorAdjust() override = default;
    int  GetBlue() const   { return m_cBlue; }
    int  GetGreen() const  { return m_cGreen; }
    int  GetRed() const    { return m_cRed; }
    BOOL IsGray() const    { return m_isGray; }

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnInitDialog() override;
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CColorAdjust(const CColorAdjust&) = delete;
    CColorAdjust& operator=(const CColorAdjust&) = delete;

    // Message handlers.
    LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member functions.
    void    CreateImagePreviews();
    BOOL    OnGrayScale();
    BOOL    OnTextChange(HWND editCtrl);
    void    Paint();
    void    UpdatePreview();

    // Member variables for dialog  controls.
    CSlider m_redSlider;
    CSlider m_greenSlider;
    CSlider m_blueSlider;
    CEdit   m_redEdit;
    CEdit   m_greenEdit;
    CEdit   m_blueEdit;
    CStatic m_preview;
    CBitmap m_image;
    CBitmap m_previewImage;
    CBitmap m_previewOrigImage;

    // Member variables
    int     m_cBlue;
    int     m_cGreen;
    int     m_cRed;
    BOOL    m_isGray;
};


#endif // _COLOURADJUST_H_

