#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H


#include "../Win32++/docking.h"
#include "Views.h"

class CPaneTopLeft : public CDockable
{
public:
	CPaneTopLeft(); 
	virtual ~CPaneTopLeft() {}
	virtual void OnInitialUpdate(); 

private:
	CViewSimple m_View;
};

class CPaneTopRight : public CDockable
{
public:
	CPaneTopRight();
	virtual ~CPaneTopRight() {}
	virtual void OnInitialUpdate(); 

private:
	CViewText m_View;
};

class CPaneBottomLeft : public CDockable
{
public:
	CPaneBottomLeft();
	virtual ~CPaneBottomLeft() {}
	virtual void OnInitialUpdate(); 

private:
	CViewTree m_View;

};

class CPaneBottomRight : public CDockable
{
public:
	CPaneBottomRight();
	virtual ~CPaneBottomRight() {}
	virtual void OnInitialUpdate(); 

private:
	CViewList m_View;

};

#endif // SPLITTERPANES_H
