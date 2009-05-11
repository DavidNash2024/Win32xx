#ifndef MYTABBEDMDI_H
#define MYTABBEDMDI_H

#include "../../Win32++/tab.h"

class CMyTabbedMDI : public CTabbedMDI
{
public:
	virtual ~CMyTabbedMDI() {}
	virtual CWnd* NewMDIChildFromID(int nID);
};

#endif

