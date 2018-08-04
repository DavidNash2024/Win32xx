
#ifndef APPHELP_H
#define APPHELP_H

#include "ContextHelp.h"
#include "HelpAbout.h"


// The CAppHelp class uses composition to combine the features of the
//  CContextHelp and CHelpAbout classes.

class CAppHelp
{
public:
    CAppHelp();
    ~CAppHelp() {}
    
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
    CHelpAbout m_helpAbout;
};


#endif // APPHELP_H
