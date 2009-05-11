////////////////////////////////////////////////////
// DockTabbedMDI.h - Declaration of the CDockTabbedMDI class


#ifndef DOCKTABBEDMDI_H
#define DOCKTABBEDMDI_H

#include "../../Win32++/tab.h"
#include "../../Win32++/docking.h"
#include "MyTabbedMDI.h"

// Declaration of the CDockTabbedMDI class
class CDockTabbedMDI : public CDocker
{
public:
	CDockTabbedMDI();
	virtual ~CDockTabbedMDI() {}
	virtual CDocker* NewDockerFromID(int nID);
	virtual void OnInitialUpdate();

private:
	CMyTabbedMDI m_View;

};

#endif // DOCKTABBEDMDI_H
