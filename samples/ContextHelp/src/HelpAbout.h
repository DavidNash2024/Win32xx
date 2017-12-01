#ifndef HELPABOUT_H
#define HELPABOUT_H


class CHelpAbout : public CDialog
{
public:
    CHelpAbout();
    virtual ~CHelpAbout() {}

    const CString& GetCredits() const   { return m_Credits; }
    void SetCredits(LPCTSTR szCredits)  { m_Credits = szCredits; }

protected:  
    virtual BOOL    OnInitDialog();

private:
    CString     m_Credits;          // AboutBox contents

};

#endif // HELPABOUT_H
