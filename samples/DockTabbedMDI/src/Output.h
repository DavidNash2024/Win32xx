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
    // Virtual functions that override base class functions.
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewOutput(const CViewOutput&);               // Disable copy construction
    CViewOutput& operator=(const CViewOutput&);    // Disable assignment operator
};


///////////////////////////////////////////////////
// CContainOutput manages a dock container. It uses
// CViewOutput as its view.
class CContainOutput : public CDockContainer
{
public:
    CContainOutput();
    virtual ~CContainOutput() {}

protected:
    // Virtual functions that override base class functions.
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CContainOutput(const CContainOutput&);              // Disable copy construction
    CContainOutput& operator=(const CContainOutput&);   // Disable assignment operator

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

protected:
    virtual void RecalcDockLayout();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDockOutput(const CDockOutput&);              // Disable copy construction
    CDockOutput& operator=(const CDockOutput&);   // Disable assignment operator

    CContainOutput m_view;
};

#endif // OUTPUT_H
