//////////////////////////////////////////////
// SplitterPanes.h

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H


#include "Views.h"


class CDockSimple : public CDocker
{
public:
	CDockSimple(); 
	virtual ~CDockSimple() {}
	virtual void OnInitialUpdate(); 

private:
	CViewSimple m_View;
};

class CDockText : public CDocker
{
public:
	CDockText();
	virtual ~CDockText() {}
	virtual void OnInitialUpdate(); 

private:
	CViewText m_View;
};

class CDockTree : public CDocker
{
public:
	CDockTree();
	virtual ~CDockTree() {}
	virtual void OnInitialUpdate(); 

private:
	CViewTree m_View;

};

class CDockList : public CDocker
{
public:
	CDockList();
	virtual ~CDockList() {}
	virtual void OnInitialUpdate(); 

private:
	CViewList m_View;

};

#endif // SPLITTERPANES_H
