///////////////////////////////////////////////////////
// SplitterPanes.h
//  Declaration of the CLeftPane and CRightPane classes


#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H


#include "../Win32++/dockable.h"
#include "MyListView.h"
#include "MyTreeView.h"

class CLeftPane : public CDockable
{
public:
    CLeftPane() { SetView(m_View); }

private:
	CMyTreeView m_View;
};


class CRightPane : public CDockable
{
public:
    CRightPane() { SetView(m_View); }
	virtual void OnInitialUpdate();

private:
	CMyListView m_View;
};



#endif // SPLITTERPANES_H

