////////////////////////////////////////////////////
// DockTabbedMDI.h - Declaration of the CDockTabbedMDI class


#ifndef DOCKTABBEDMDI_H
#define DOCKTABBEDMDI_H

#include "../../Win32++/tab.h"
#include "../../Win32++/docking.h"


// Declaration of the CDockTabbedMDI class
class CDockTabbedMDI : public CDocker
{
public:
	CDockTabbedMDI();
	virtual ~CDockTabbedMDI() {}
	virtual void OnInitialUpdate();

private:
	CTabbedMDI m_View;

};

#endif // DOCKTABBEDMDI_H
