/////////////////////////////
// Dockers.h
//

#ifndef DOCKABLES_H
#define DOCKABLES_H


#include "Views.h"

///////////////////////////////////////
// Declaration of the CDockSimple class
//
class CDockSimple : public CDocker
{
public:
    CDockSimple();
    virtual ~CDockSimple() {}

private:
    CViewSimple m_view;
};

////////////////////////////////////////
// Declaration of the CDockClasses class
//
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() {}

private:
    CViewTree m_view;
};

//////////////////////////////////////
// Declaration of the CDockFiles class
//
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

private:
    CViewList m_view;
};

/////////////////////////////////////
// Declaration of the CDockText class
//
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() {}

private:
    CViewText m_view;
};


#endif // DOCKABLES_H

