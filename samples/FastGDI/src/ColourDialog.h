//////////////////////////////////////////////
// ColourDialog.h

#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT resID, CBitmap& image);
    virtual ~CColourDialog();
    int  GetBlue() const   { return m_cBlue; }
    int  GetGreen() const  { return m_cGreen; }
    int  GetRed() const    { return m_cRed; }
    BOOL IsGray() const    { return m_isGray; }

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnInitDialog();
    virtual void    OnOK();
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void    CreateImagePreviews();
    void    OnGrayScale();
    LRESULT OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam);
    void    OnTextChange(HWND editCtrl);
    void    Paint();
    void    UpdatePreview();

    // CWnd object for the dialog controls to attach to
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
    int     m_cBlue;
    int     m_cGreen;
    int     m_cRed;
    BOOL    m_isGray;
};


#endif // COLOURDIALOG_H

