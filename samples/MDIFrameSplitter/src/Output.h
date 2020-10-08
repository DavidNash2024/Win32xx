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
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
};


////////////////////////////////////////////////////
// CContainOutput manages a dock container. It has a
// CViewOutput view.
class CContainOutput : public CDockContainer
{
public:
    CContainOutput()
    {
        SetView(m_viewOutput);
        SetDockCaption (_T("Output View - Docking container"));
        SetTabText(_T("Output"));
        SetTabIcon(IDI_TEXT);
    }
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
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CContainOutput m_view;
};

#endif // OUTPUT_H
