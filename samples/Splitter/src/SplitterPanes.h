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

protected:
    virtual LRESULT OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDockText(const CDockText&);                // Disable copy construction
    CDockText& operator = (const CDockText&);   // Disable assignment operator

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

protected:
    // Virtual functions overriding base class functions
    virtual LRESULT OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDockTree(const CDockTree&);                // Disable copy construction
    CDockTree& operator = (const CDockTree&);   // Disable assignment operator

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

protected:
    virtual LRESULT OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDockList(const CDockList&);                // Disable copy construction
    CDockList& operator = (const CDockList&);   // Disable assignment operator

    CViewList m_view;
};

#endif // SPLITTERPANES_H
