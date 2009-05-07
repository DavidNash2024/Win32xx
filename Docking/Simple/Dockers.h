/////////////////////////////////////////////
// Dockers.h

#ifndef DOCKABLES_H
#define DOCKABLES_H

#include "../../Win32++/docking.h"
#include "Views.h"

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

