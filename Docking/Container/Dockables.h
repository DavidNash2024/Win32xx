/////////////////////////////////////////////
// Dockables.h

#ifndef DOCKABLES_H
#define DOCKABLES_H

#include "../../Win32++/dockable.h"
#include "Views.h"
#include "Classes.h"
#include "Files.h"

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

// Declaration of the CDockClassContainer class
class CDockClassContainer : public CDockable
{
public:
	CDockClassContainer(); 
	virtual ~CDockClassContainer() {}
	virtual void OnInitialUpdate();

//private:
	CContainClasses m_Classes;

};

// Declaration of the CDockFileContainer class
class CDockFileContainer : public CDockable
{
public:
	CDockFileContainer(); 
	virtual ~CDockFileContainer() {}
	virtual void OnInitialUpdate();

//private:
	CContainFiles m_Files;

};


#endif // DOCKABLES_H

