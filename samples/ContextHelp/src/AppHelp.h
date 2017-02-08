
#ifndef APPHELP_H
#define APPHELP_H

#include "ContextHelp.h"
#include "HelpAbout.h"


// The CAppHelp class derives from the CContextHelp class in 
// order to implement application-specific features. A CFrame-based
// architecture is assumed.

class CAppHelp : public CContextHelp
{
public:
	CAppHelp();
	~CAppHelp() {}

	void SetCommandHelpTopics();
	void SetClientHelpTopics();
	void SetFrameHelpTopics();

private:
};


#endif // APPHELP_H
