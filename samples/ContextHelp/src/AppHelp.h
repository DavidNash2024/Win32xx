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
    virtual ~CHelp() = default;

    BOOL About(HWND hParent = nullptr);
    void SetCredits(LPCWSTR credits);
    void SetHelpFilePath(LPCWSTR chmName);
    void ShowHelpTopic(UINT id);
    void ShowHelpTopic(LPCWSTR topic);

private:
    void AddHelpTopic(UINT id, LPCWSTR topic);
    void SetCommandHelpTopics();
    void SetClientHelpTopics();
    void SetFrameHelpTopics();

    // Member variables
    CContextHelp m_contextHelp;
    CAboutHelp m_helpAbout;
};


#endif // APPHELP_H
