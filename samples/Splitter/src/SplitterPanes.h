/////////////////////////////
// SplitterPanes.h
//

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H

#include "Views.h"


////////////////////////////////////////////////////
// CDockTest manages the docker which uses CViewText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() {}

private:
    CViewText m_view;
};


///////////////////////////////////////////////////////////
// CDockTree manages the docker which uses CViewTree as its
// view.
class CDockTree : public CDocker
{
public:
    CDockTree();
    virtual ~CDockTree() {}

private:
    CViewTree m_view;
};


////////////////////////////////////////////////////
// CDockList manages the docker which uses CViewList
// as its view.
class CDockList : public CDocker
{
public:
    CDockList();
    virtual ~CDockList() {}

private:
    CViewList m_view;
};

#endif // SPLITTERPANES_H
