/////////////////////////////
// SplitterPanes.h
//

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H

#include "Views.h"

///////////////////////////////////////
// Declaration for the CDockText class.
//
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() {}

private:
    CViewText m_view;
};

///////////////////////////////////////
// Declaration for the CDockTree class.
//
class CDockTree : public CDocker
{
public:
    CDockTree();
    virtual ~CDockTree() {}

private:
    CViewTree m_view;
};

///////////////////////////////////////
// Declaration for the CDockList class.
//
class CDockList : public CDocker
{
public:
    CDockList();
    virtual ~CDockList() {}

private:
    CViewList m_view;
};

#endif // SPLITTERPANES_H
