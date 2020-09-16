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
    CViewText m_view;
};


#endif // DOCKABLES_H

