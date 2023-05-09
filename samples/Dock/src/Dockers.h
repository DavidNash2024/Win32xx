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

private:
    CDockSimple(const CDockSimple&);                // Disable copy construction
    CDockSimple& operator = (const CDockSimple&);   // Disable assignment operator

    CViewSimple m_view;
};


///////////////////////////////////////////////////////
// CDockClasses manages the docker which uses CViewTree
// as its view.
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() {}

private:
    CDockClasses(const CDockClasses&);                // Disable copy construction
    CDockClasses& operator = (const CDockClasses&);   // Disable assignment operator

    CViewTree m_view;
};


/////////////////////////////////////////////////////
// CDockFiles manages the docker which uses CViewList
// as its view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

private:
    CDockFiles(const CDockFiles&);                // Disable copy construction
    CDockFiles& operator = (const CDockFiles&);   // Disable assignment operator

    CViewList m_view;
};


////////////////////////////////////////////////////
// CDockText manages the docker which uses CViewText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() {}

private:
    CDockText(const CDockText&);                // Disable copy construction
    CDockText& operator = (const CDockText&);   // Disable assignment operator

    CViewText m_view;
};


#endif // DOCKABLES_H

