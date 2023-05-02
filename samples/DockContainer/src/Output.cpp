///////////////////////////////////////////////////
// Output.cpp -  Definitions for the CViewOutput,
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
    SetDPIFont();
    SetWindowText(_T("Output Window"));
    SetReadOnly();
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewOutput::OnDPIChangedAfterParent(UINT, WPARAM, LPARAM)
{
    SetDPIFont();
    return 0;
}

// Adjusts the font size in response to window DPI changes.
void CViewOutput::SetDPIFont()
{
    m_font.CreatePointFont(100, _T("Courier New"));
    m_font = DPIScaleFont(m_font, 9);
    SetFont(m_font);
}

// Process the window messages sent to this message.
LRESULT CViewOutput::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DPICHANGED_AFTERPARENT: return OnDPIChangedAfterParent(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
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
