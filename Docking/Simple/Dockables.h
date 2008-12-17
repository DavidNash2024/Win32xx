/////////////////////////////////////////////
// Dockables.h

#ifndef DOCKABLES_H
#define DOCKABLES_H

#include "../../Win32++/dockable.h"
#include "Views.h"

// Declaration of the CDockSimple class
class CDockSimple : public CDockable
{
public:
	CDockSimple();
	virtual ~CDockSimple() {}
	virtual void OnInitialUpdate();

private:
	CViewSimple m_View;

};

// Declaration of the CDockClass class
class CDockClass : public CDockable
{
public:
	CDockClass();
	virtual ~CDockClass() {}
	virtual void OnInitialUpdate();

private:
	CViewTree m_View;

};

// Declaration of the CDockFiles class
class CDockFiles : public CDockable
{
public:
	CDockFiles();
	virtual ~CDockFiles() {}
	virtual void OnInitialUpdate();

private:
	CViewList m_View;

};

// Declaration of the CDockText class
class CDockText : public CDockable
{
public:
	CDockText();
	virtual ~CDockText() {}
	virtual void OnInitialUpdate();

private:
	CViewText m_View;

};


#endif // DOCKABLES_H

