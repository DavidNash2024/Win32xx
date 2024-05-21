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
    virtual ~CDockSimple() {}

protected:
    // Virtual functions overriding base class functions.
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CDockSimple(const CDockSimple&);               // Disable copy construction
    CDockSimple& operator=(const CDockSimple&);    // Disable assignment operator

    CViewSimple m_view;
};


///////////////////////////////////////////////////////
// CDockClasses manages the docker which uses CViewClasses
// as its view.
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() {}

protected:
    // Virtual functions overriding base class functions.
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CDockClasses(const CDockClasses&);               // Disable copy construction
    CDockClasses& operator=(const CDockClasses&);    // Disable assignment operator

    CViewClasses m_view;
};


/////////////////////////////////////////////////////
// CDockFiles manages the docker which uses CViewFiles
// as its view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

protected:
    // Virtual functions overriding base class functions.
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CDockFiles(const CDockFiles&);               // Disable copy construction
    CDockFiles& operator=(const CDockFiles&);    // Disable assignment operator

    CViewFiles m_view;
};


////////////////////////////////////////////////////
// CDockText manages the docker which uses CViewText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() {}

protected:
    // Virtual functions overriding base class functions.
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CDockText(const CDockText&);               // Disable copy construction
    CDockText& operator=(const CDockText&);    // Disable assignment operator

    CViewText m_view;
};


#endif // DOCKABLES_H

