///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput,
//          and CDockOutput classes
//

#ifndef OUTPUT_H
#define OUTPUT_H

#include "resource.h"


///////////////////////////////////////////////////////////
// CViewOutput manages an edit control. It displays output.
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

private:
    CViewOutput(const CViewOutput&);                // Disable copy construction
    CViewOutput& operator = (const CViewOutput&);   // Disable assignment operator
};


///////////////////////////////////////////////////
// CContainOutput manages a dock container. It uses
// CViewOutput as its view.
class CContainOutput : public CDockContainer
{
public:
    CContainOutput();
    virtual ~CContainOutput() {}

private:
    CContainOutput(const CContainOutput&);                // Disable copy construction
    CContainOutput& operator = (const CContainOutput&);   // Disable assignment operator

    CViewOutput m_viewOutput;
};


///////////////////////////////////////////////////////
// CDockOutput manages a docker. It uses CContainOutput
// as its view.
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() {}

private:
    CDockOutput(const CDockOutput&);                // Disable copy construction
    CDockOutput& operator = (const CDockOutput&);   // Disable assignment operator

    CContainOutput m_view;
};


#endif // OUTPUT_H

