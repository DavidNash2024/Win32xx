///////////////////////////////////////////////////////
// SplitterPanes.h
//

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H

#include "MyListView.h"
#include "MyTreeView.h"

/////////////////////////////////////
// Declaration of the CLeftPane class
//
class CLeftPane : public CDocker
{
public:
    CLeftPane();
    virtual ~CLeftPane() {}

private:
    CMyTreeView m_view;
};

//////////////////////////////////////
// Declaration of the CRightPane class
//
class CRightPane : public CDocker
{
public:
    CRightPane();
    virtual ~CRightPane() {}

private:
    CMyListView m_view;
};

#endif // SPLITTERPANES_H

