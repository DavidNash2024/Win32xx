/////////////////////////////
// Dockers.cpp
//

#include "stdafx.h"
#include "Dockers.h"


////////////////////////////////////
//  CDockSimple function definitions
//

// Constructor.
CDockSimple::CDockSimple()
{
    // Set the view window for the simple docker.
    SetView(m_view);
    SetCaption (_T("Simple View - Docking"));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockSimple::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}


/////////////////////////////////
// CDockText function definitions
//

// Constructor.
CDockText::CDockText()
{
    // Set the view window to our edit control.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));

    SetCaption(_T("Text View - Docking"));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockText::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}


////////////////////////////////////
// CDockClasses function definitions
//

// Constructor.
CDockClasses::CDockClasses()
{
    // Set the view window to our TreeView control.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));

    SetCaption(_T("Class View - Docking"));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockClasses::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    RecalcDockLayout();
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}


///////////////////////////////////
//  CDockFiles function definitions
//

// Constructor.
CDockFiles::CDockFiles()
{
    // Set the view window to our ListView control.
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));

    SetCaption(_T("Files View - Docking"));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockFiles::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}
