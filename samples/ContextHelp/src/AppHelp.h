
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
	
	void About();
	void AddHelpTopic(UINT nID, LPCTSTR topic);
	void SetCommandHelpTopics();
	void SetClientHelpTopics();
	void SetCredits(LPCTSTR szCredits);
	void SetFrameHelpTopics();
	void SetHelpFilePath(LPCTSTR chmName);
	void ShowHelpTopic(UINT nID);
	void ShowHelpTopic(LPCTSTR topic);

private:
	CContextHelp m_ContextHelp;
	CHelpAbout m_HelpAbout;
};


#endif // APPHELP_H
