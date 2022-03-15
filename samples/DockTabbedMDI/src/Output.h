///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput,
//          and CDockOutput classes
//

#ifndef OUTPUT_H
#define OUTPUT_H

#include <richedit.h>


////////////////////////////////////////////////////////////////
// CViewOutput manages an rich edit control. It displays output.
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
    CFont m_font;
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
    CViewOutput m_viewOutput;
};


//////////////////////////////////////////
// CDockOutput manages a docker that uses
// CContainOutput as its view.
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() {}

private:
    CContainOutput m_view;
};

#endif // OUTPUT_H
