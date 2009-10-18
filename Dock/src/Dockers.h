/////////////////////////////////////////////
// Dockers.h

#ifndef DOCKABLES_H
#define DOCKABLES_H


#include "Views.h"


enum DockIDs
{
	ID_DOCK_CLASSES1 = 1,
	ID_DOCK_CLASSES2 = 2,
	ID_DOCK_FILES1 = 3,
	ID_DOCK_FILES2 = 4,
	ID_DOCK_SIMPLE1 = 5,
	ID_DOCK_SIMPLE2 = 6,
	ID_DOCK_TEXT1 = 7,
	ID_DOCK_TEXT2 = 8
};


// Declaration of the CDockSimple class
class CDockSimple : public CDocker
{
public:
	CDockSimple();
	virtual ~CDockSimple() {}

protected:
	virtual CDocker* NewDockerFromID(int nID);
	virtual void OnInitialUpdate();

private:
	CViewSimple m_View;

};

// Declaration of the CDockClasses class
class CDockClasses : public CDocker
{
public:
	CDockClasses();
	virtual ~CDockClasses() {}
	virtual void OnInitialUpdate();

private:
	CViewTree m_View;

};

// Declaration of the CDockFiles class
class CDockFiles : public CDocker
{
public:
	CDockFiles();
	virtual ~CDockFiles() {}
	virtual void OnInitialUpdate();

private:
	CViewList m_View;

};

// Declaration of the CDockText class
class CDockText : public CDocker
{
public:
	CDockText();
	virtual ~CDockText() {}
	virtual void OnInitialUpdate();

private:
	CViewText m_View;

};


#endif // DOCKABLES_H

