/////////////////////////////////////////
// AboutBox.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef ABOUT_BOX_H_DEFINED
#define ABOUT_BOX_H_DEFINED

///////////////////////////////////////////////////////////////////////////////
// The about dialog box. It is responsible for displaying the about information
// about the application. It is derived from CDialog, which provides the basic
// dialog functionality for the about box.
class AboutBox : public CDialog
{
public:
    AboutBox();
    virtual ~AboutBox() override = default;

    void SetStatus(const CString& status) { m_status = status; }

protected:
    virtual BOOL OnInitDialog() override;

private:
    AboutBox(const AboutBox&) = delete;
    AboutBox& operator=(const AboutBox&) = delete;

    CString m_status;
};

#endif // ABOUT_BOX_H_DEFINED
