#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "../Win32++/wincore.h"

class CProgressbar : public CWnd
{
public:
	CProgressbar() {}
	virtual ~CProgressbar() {}
	
	void SetProgress(int nPos);
};


#endif
