/////////////////////////////////////////
// AboutBox.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef ABOUTBOX_H
#define ABOUTBOX_H

/////////////////////////////////////////////////
// This class implements the AboutBox dialog box.
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
