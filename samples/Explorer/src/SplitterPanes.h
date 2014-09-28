///////////////////////////////////////////////////////
// SplitterPanes.h
//  Declaration of the CLeftPane and CRightPane classes

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H


#include "MyListView.h"
#include "MyTreeView.h"

class CLeftPane : public CDocker
{
public:
    CLeftPane() { SetView(m_View); }

private:
	CMyTreeView m_View;
};


class CRightPane : public CDocker
{
public:
    CRightPane() { SetView(m_View); }
	virtual int OnCreate(LPCREATESTRUCT pcs);

private:
	CMyListView m_View;
};



#endif // SPLITTERPANES_H

