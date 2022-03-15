///////////////////////////////////////////////////
// Output.cpp -  Definitions for the CViewOutput, CContainOutput
//              and CDockOutput classes


#include "stdafx.h"
#include "Output.h"
#include "resource.h"


///////////////////////////////////
// CViewOutput function definitions
//

// Constructor.
CViewOutput::CViewOutput()
{
}

// Destructor.
CViewOutput::~CViewOutput()
{
}

// Called when the window handle (HWND) is attached to CViewOutput.
void CViewOutput::OnAttach()
{
    SetWindowText(_T("Output Window"));
}

// Set the CREATESTRUCT parameters before the window is created.
void CViewOutput::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}


///////////////////////////////////
// CDockOutput function definitions
//

// Constructor.
CDockOutput::CDockOutput()
{
    // Set the view window to our edit control
    SetView(m_view);
}

// Called when the window is created.
int CDockOutput::OnCreate(CREATESTRUCT& cs)
{
    // Set the width of the splitter bar
    SetBarWidth(8);

    return CDocker::OnCreate(cs);
}


//////////////////////////////////////
// CContainOutput function definitions
//

// Constructor.
CContainOutput::CContainOutput()
{
    SetView(m_viewOutput);
    SetDockCaption(_T("Output View - Docking container"));
    SetTabText(_T("Output"));
    SetTabIcon(IDI_TEXT);
}


