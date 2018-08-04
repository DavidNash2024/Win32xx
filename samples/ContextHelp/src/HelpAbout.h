#ifndef HELPABOUT_H
#define HELPABOUT_H


class CHelpAbout : public CDialog
{
public:
    CHelpAbout();
    virtual ~CHelpAbout() {}

    const CString& GetCredits() const   { return m_credits; }
    void SetCredits(LPCTSTR credits)  { m_credits = credits; }

protected:  
    virtual BOOL    OnInitDialog();

private:
    CString     m_credits;          // AboutBox contents

};

#endif // HELPABOUT_H
