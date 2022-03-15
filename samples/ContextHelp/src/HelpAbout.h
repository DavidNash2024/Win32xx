/////////////////////////////
// HelpAbout.h
//

#ifndef HELPABOUT_H
#define HELPABOUT_H

/////////////////////////////////////////////////////////////////
// CAboutHelp manages the dialog which displays information about
// this application.
class CAboutHelp : public CDialog
{
public:
    CAboutHelp();
    virtual ~CAboutHelp() {}

    const CString& GetCredits() const { return m_credits; }
    void SetCredits(LPCTSTR credits) { m_credits = credits; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnInitDialog();

private:
    CString     m_credits;          // AboutBox contents
};

#endif // HELPABOUT_H
