////////////////////////////////////////////////////
// DockTabbedMDI.h - Declaration of the CDockTabbedMDI class


#ifndef DOCKTABBEDMDI_H
#define DOCKTABBEDMDI_H


#include "MyTabbedMDI.h"


enum DockIDs
{
	ID_DOCK_CLASSES1 = 1,
	ID_DOCK_CLASSES2 = 2,
	ID_DOCK_FILES1 = 3,
	ID_DOCK_FILES2 = 4,
	ID_DOCK_OUTPUT1 = 5,
	ID_DOCK_OUTPUT2 = 6,
	ID_DOCK_TEXT1 = 7,
	ID_DOCK_TEXT2 = 8,
	ID_DOCK_BROWSER = 9,
	ID_DOCK_RECT = 10
};

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
