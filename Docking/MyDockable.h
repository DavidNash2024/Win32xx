/////////////////////////////////////////////
// MyDockable.h

#ifndef MYDOCKABLE_H
#define MYDOCKABLE_H

#include "../Win32++/Dockable.h"
#include "View.h"

class CMyDockable : public CDockable
{
public:
	CMyDockable();
	virtual ~CMyDockable() {}
private:

	CView m_View;
};


#endif // MYDOCKABLE_H

