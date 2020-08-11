/////////////////////////////
// HelpAbout.h
//

#ifndef HELPABOUT_H
#define HELPABOUT_H

//////////////////////////////////////
// Declaration of the CAboutHelp class
//
class CAboutHelp : public CDialog
{
public:
    CAboutHelp();
    virtual ~CAboutHelp() {}

    const CString& GetCredits() const   { return m_credits; }
    void SetCredits(LPCTSTR credits)  { m_credits = credits; }

protected:
    virtual BOOL    OnInitDialog();

private:
    CString     m_credits;          // AboutBox contents

};

#endif // HELPABOUT_H
