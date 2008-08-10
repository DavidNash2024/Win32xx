/////////////////////////////////////////////
// MyDockable.h

#ifndef MYDOCKABLE_H
#define MYDOCKABLE_H

#include "../Win32++/Dockable.h"
#include "View.h"

class CMyDockable : public CDockable
{
public:
	CMyDockable() { m_pView = new CView; }
	~CMyDockable() { delete m_pView; }
private:

};


#endif // MYDOCKABLE_H