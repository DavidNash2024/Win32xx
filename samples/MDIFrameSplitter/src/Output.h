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
    virtual ~CViewOutput() override;

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewOutput(const CViewOutput&) = delete;
    CViewOutput& operator=(const CViewOutput&) = delete;
};


////////////////////////////////////////////////////
// CContainOutput manages a dock container. It has a
// CViewOutput view.
class CContainOutput : public CDockContainer
{
public:
    CContainOutput();
    virtual ~CContainOutput() override {}

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
private:
    CContainOutput(const CContainOutput&) = delete;
    CContainOutput& operator=(const CContainOutput&) = delete;

    CViewOutput m_viewOutput;
};


//////////////////////////////////////////
// CDockOutput manages a docker that has a
// CContainOutput view.
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() override {}

protected:
    virtual int OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockOutput(const CDockOutput&) = delete;
    CDockOutput& operator=(const CDockOutput&) = delete;

    CContainOutput m_view;
};

#endif // OUTPUT_H
