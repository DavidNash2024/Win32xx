/////////////////////////////////////////////
// DockContainer.h

#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H

#include "../../Win32++/dockable.h"
#include "Classes.h"
#include "Files.h"

// Declaration of the CDockContainer class
class CDockContainer : public CDockable
{
public:
	CDockContainer() { SetView(m_Classes); };
	virtual ~CDockContainer() {}
	virtual void OnInitialUpdate() {}

private:
	CContainClasses m_Classes;
	CContainFiles m_Files;

};

#endif // 

