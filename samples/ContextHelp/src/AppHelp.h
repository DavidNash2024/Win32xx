/////////////////////////////
// AppHelp.h
//

#ifndef APPHELP_H
#define APPHELP_H

#include "ContextHelp.h"
#include "HelpAbout.h"


//////////////////////////////////////////////////////
// CHelp combines the features of the CContextHelp and
// CAboutHelp classes.
class CHelp
{
public:
    CHelp();
    virtual ~CHelp() {}

    BOOL About(HWND hParent = 0);
    void SetCredits(LPCTSTR credits);
    void SetHelpFilePath(LPCTSTR chmName);
    void ShowHelpTopic(UINT id);
    void ShowHelpTopic(LPCTSTR topic);

private:
    void AddHelpTopic(UINT id, LPCTSTR topic);
    void SetCommandHelpTopics();
    void SetClientHelpTopics();
    void SetFrameHelpTopics();

    // Member variables
    CContextHelp m_contextHelp;
    CAboutHelp m_helpAbout;
};


#endif // APPHELP_H
