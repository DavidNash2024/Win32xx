///////////////////////////////////////////////////////
// SplitterPanes.h
//

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H

#include "MyListView.h"
#include "MyTreeView.h"


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

private:
    CMyListView m_view;
};

#endif // SPLITTERPANES_H

