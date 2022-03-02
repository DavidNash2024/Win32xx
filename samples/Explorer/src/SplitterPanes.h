///////////////////////////////////////////////////////
// SplitterPanes.h
//

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H

#include "MyListView.h"
#include "MyTreeView.h"

// The docker identifiers (dock IDs)
const int ID_DOCK_LEFTPANE = 1;


////////////////////////////////////////////////////
// CLeftPane is a docker that manages the left view.
// CLeftPane's view window is CMyTreeView.
class CLeftPane : public CDocker
{
public:
    CLeftPane();
    virtual ~CLeftPane() {}

private:
    CMyTreeView m_view;
};


//////////////////////////////////////////////////////
// CRightPane is a docker that manages the right view.
// CRightPane's view window is CMyListView.
class CRightPane : public CDocker
{
public:
    CRightPane();
    virtual ~CRightPane() {}
    virtual CDocker* NewDockerFromID(int id);

private:
    CMyListView m_view;
};

#endif // SPLITTERPANES_H

