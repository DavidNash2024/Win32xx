//////////////////////////////////////////////
// ColourDialog.h

#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H


/////////////////////////////////////////////////////
// CColourDialog manages a dialog that allows the
// user to choose the color adjustment for the image.
class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT resID, CBitmap& image);
    virtual ~CColourDialog() override;
    int  GetBlue() const   { return m_cBlue; }
    int  GetGreen() const  { return m_cGreen; }
    int  GetRed() const    { return m_cRed; }
    BOOL IsGray() const    { return m_isGray; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnInitDialog() override;
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CColourDialog(const CColourDialog&) = delete;
    CColourDialog& operator=(const CColourDialog&) = delete;

    void    CreateImagePreviews();
    BOOL    OnGrayScale();
    LRESULT OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam);
    BOOL    OnTextChange(HWND editCtrl);
    void    Paint();
    void    UpdatePreview();

    // Member variables for dialog  controls
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


#endif // COLOURDIALOG_H

