///////////////////////////////////////////////////
// Output.cpp -  Definitions for the CViewOutput, CContainOutput
//              and CDockOutput classes
//


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

// Called when a window handle (HWND) is attached to CViewOutput.
void CViewOutput::OnAttach()
{
    CFont font;
    font.CreatePointFont(100, _T("Courier New"));
    SetFont(font);
    SetWindowText(_T("Read Only Output Window"));
}

// Sets the CREATESTRUCT parameters before the window is created.
void CViewOutput::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL | ES_READONLY;
}


//////////////////////////////////////
// CContainOutput function definitions
//

// Constructor.
CContainOutput::CContainOutput()
{
    SetView(m_viewOutput);
    SetDockCaption (_T("Output View - Docking container"));
    SetTabText(_T("Output"));
    SetTabIcon(IDI_TEXT);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CContainOutput::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDockContainer::PreCreate(cs);

    // Add the WS_EX_COMPOSITED to reduce flicker.
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}



///////////////////////////////////
// CDockOutput function definitions
//

// Constructor.
CDockOutput::CDockOutput()
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);
}
