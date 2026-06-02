/////////////////////////////
// NewDialog.h
//

#ifndef NEWDIALOG_H
#define NEWDIALOG_H


///////////////////////////////////////////////////////
// CNewDialog displays a dialog to choose a new toolbar.
//
class CNewDialog : public CDialog
{
public:
    CNewDialog(UINT resID);
    virtual ~CNewDialog() override = default;

    int GetColorBits() const;
    int GetImageSize() const;

protected:
    virtual BOOL    OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;

private:
    CNewDialog(const CNewDialog&) = delete;
    CNewDialog& operator=(const CNewDialog&) = delete;

    // Command handlers
    bool OnRangeOfImageSizes(UINT idFirst, UINT idLast, UINT idClicked);
    bool OnRangeOfColorBits(UINT idFirst, UINT idLast, UINT idClicked);

    int m_imageSizeRadio;
    int m_colorBitsRadio;
};

#endif // NEWDIALOG_H
