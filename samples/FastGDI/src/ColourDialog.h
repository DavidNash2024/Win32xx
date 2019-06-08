//////////////////////////////////////////////
// ColourDialog.h

#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT resID, CBitmap& image);
    virtual ~CColourDialog();
    void CreateImagePreviews();
    
protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnInitDialog();
    virtual void    OnOK();
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void OnGrayScale();
    LRESULT OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam);
    void    Paint();

    // CWnd object for the dialog controls to attach to
    CWnd m_redSlider;
    CWnd m_greenSlider;
    CWnd m_blueSlider;
    CWnd m_redEdit;
    CWnd m_greenEdit;
    CWnd m_blueEdit;
    CWnd m_preview;

    CBitmap m_image;
    CBitmap m_previewImage;
    CBitmap m_previewOrigImage;
    int m_cRed;
    int m_cGreen;
    int m_cBlue;
};


#endif // COLOURDIALOG_H

