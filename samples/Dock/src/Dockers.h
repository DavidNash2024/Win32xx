/////////////////////////////
// Dockers.h
//

#ifndef DOCKABLES_H
#define DOCKABLES_H

#include "Views.h"


/////////////////////////////////////////////////////
// CDockSimple manages the docker with a simple view.
class CDockSimple : public CDocker
{
public:
    CDockSimple();
    virtual ~CDockSimple() override {}

protected:
    // Virtual functions overriding base class functions.
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockSimple(const CDockSimple&) = delete;
    CDockSimple& operator=(const CDockSimple&) = delete;

    CViewSimple m_view;
};


///////////////////////////////////////////////////////
// CDockClasses manages the docker which uses CViewClasses
// as its view.
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() override {}

protected:
    // Virtual functions overriding base class functions.
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockClasses(const CDockClasses&) = delete;
    CDockClasses& operator=(const CDockClasses&) = delete;

    CViewClasses m_view;
};


/////////////////////////////////////////////////////
// CDockFiles manages the docker which uses CViewFiles
// as its view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() override {}

protected:
    // Virtual functions overriding base class functions.
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockFiles(const CDockFiles&) = delete;
    CDockFiles& operator=(const CDockFiles&) = delete;

    CViewFiles m_view;
};


////////////////////////////////////////////////////
// CDockText manages the docker which uses CViewText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() override {}

protected:
    // Virtual functions overriding base class functions.
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockText(const CDockText&) = delete;
    CDockText& operator=(const CDockText&) = delete;

    CViewText m_view;
};


#endif // DOCKABLES_H

