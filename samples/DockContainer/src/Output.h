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
    CViewOutput() = default;
    virtual ~CViewOutput() override = default;
    void SetDPIFont();

protected:
    // Virtual functions that override base class functions.
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewOutput(const CViewOutput&) = delete;
    CViewOutput& operator=(const CViewOutput&) = delete;

    CFont m_font;
};


///////////////////////////////////////////////////
// CContainOutput manages a dock container. It uses
// CViewOutput as its view.
class CContainOutput : public CDockContainer
{
public:
    CContainOutput();
    virtual ~CContainOutput() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CContainOutput(const CContainOutput&) = delete;
    CContainOutput& operator=(const CContainOutput&) = delete;

    CViewOutput m_viewOutput;
};


///////////////////////////////////////////////////////
// CDockOutput manages a docker. It uses CContainOutput
// as its view.
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() override = default;

protected:
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockOutput(const CDockOutput&) = delete;
    CDockOutput& operator=(const CDockOutput&) = delete;

    CContainOutput m_view;
};


#endif // OUTPUT_H
