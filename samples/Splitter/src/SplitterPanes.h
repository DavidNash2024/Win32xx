/////////////////////////////
// SplitterPanes.h
//

#ifndef SPLITTERPANES_H
#define SPLITTERPANES_H

#include "Views.h"


////////////////////////////////////////////////////
// CDockTest manages the docker that uses CViewText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() override = default;

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockText(const CDockText&) = delete;
    CDockText& operator=(const CDockText&) = delete;

    CViewText m_view;
};


///////////////////////////////////////////////////////////
// CDockTree manages the docker that uses CViewTree as its
// view.
class CDockTree : public CDocker
{
public:
    CDockTree();
    virtual ~CDockTree() override = default;

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockTree(const CDockTree&) = delete;
    CDockTree& operator=(const CDockTree&) = delete;

    CViewTree m_view;
};


////////////////////////////////////////////////////
// CDockList manages the docker that uses CViewList
// as its view.
class CDockList : public CDocker
{
public:
    CDockList();
    virtual ~CDockList() override = default;

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockList(const CDockList&) = delete;
    CDockList& operator=(const CDockList&) = delete;

    CViewList m_view;
};

#endif // SPLITTERPANES_H
