///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput,
//          and CDockOutput classes
//

#ifndef OUTPUT_H
#define OUTPUT_H


#include "resource.h"

///////////////////////////////////////
// Declaration of the CViewOutput class
//
class CViewOutput : public CRichEdit
{
public:
    CViewOutput();
    virtual ~CViewOutput();

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

};

//////////////////////////////////////////
// Declaration of the CContainOutput class
//
class CContainOutput : public CDockContainer
{
public:
    CContainOutput();
    ~CContainOutput() {}

private:
    CViewOutput m_viewOutput;
};

///////////////////////////////////////
// Declaration of the CDockOutput class
//
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() {}

private:
    CContainOutput m_view;
};


#endif // OUTPUT_H

