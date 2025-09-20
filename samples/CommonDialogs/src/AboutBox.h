/* (26-Mar-2025)                                                  (AboutBox.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the AboutBox class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes.

*******************************************************************************/

#ifndef ABOUTBOX_H
#define ABOUTBOX_H


class AboutBox : public CDialog
{
public:
    AboutBox() : CDialog(IDW_ABOUT) {}
    virtual ~AboutBox() override = default;
    void SetStatus(const CString& about) { m_about = about; }

protected:
    virtual BOOL OnInitDialog() override;

private:
    AboutBox(const AboutBox&) = delete;
    AboutBox& operator=(const AboutBox&) = delete;

    CString m_about;
};

#endif  // ABOUTBOX_H
