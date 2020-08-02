
#ifndef APPHELP_H
#define APPHELP_H

#include "ContextHelp.h"
#include "HelpAbout.h"


// The CHelp class combines the features of the CContextHelp and
//  CAboutHelp classes.

class CHelp
{
public:
    CHelp();
    ~CHelp() {}

    void About(HWND hParent = 0);
    void AddHelpTopic(UINT id, LPCTSTR topic);
    void SetCommandHelpTopics();
    void SetClientHelpTopics();
    void SetCredits(LPCTSTR credits);
    void SetFrameHelpTopics();
    void SetHelpFilePath(LPCTSTR chmName);
    void ShowHelpTopic(UINT id);
    void ShowHelpTopic(LPCTSTR topic);

private:
    CContextHelp m_contextHelp;
    CAboutHelp m_helpAbout;
};


#endif // APPHELP_H
