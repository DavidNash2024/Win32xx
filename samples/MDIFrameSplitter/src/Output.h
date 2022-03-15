///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput,
//          and CDockOutput classes

#ifndef OUTPUT_H
#define OUTPUT_H

#include <richedit.h>
#include "resource.h"


///////////////////////////////////////////////////////////////
// CViewOutput manages a rich edit control. It displays output.
// This is the view window for CContainOutput.
class CViewOutput : public CRichEdit
{
public:
    CViewOutput();
    virtual ~CViewOutput();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
};


////////////////////////////////////////////////////
// CContainOutput manages a dock container. It has a
// CViewOutput view.
class CContainOutput : public CDockContainer
{
public:
    CContainOutput();
    virtual ~CContainOutput() {}

private:
    CViewOutput m_viewOutput;
};


//////////////////////////////////////////
// CDockOutput manages a docker that has a
// CContainOutput view.
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() {}

protected:
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CContainOutput m_view;
};

#endif // OUTPUT_H
