/////////////////////////////
// SplitterPanes.cpp
//

#include "stdafx.h"
#include "SplitterPanes.h"

//////////////////////////////////
// CDockText function definitions.
//

// CDocktext constructor.
CDockText::CDockText()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(4));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockText::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(4));
    m_view.SetDPIFont();
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}


//////////////////////////////////
// CDockTree function definitions.
//

// CDockTree constructor.
CDockTree::CDockTree()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockTree::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    m_view.SetDPIImages();
    RecalcDockLayout();
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}


//////////////////////////////////
// CDockList function definitions.
//

// CDockList constructor.
CDockList::CDockList()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(4));
}

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockList::OnDPIChangedAfterParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(4));
    m_view.SetDPIImages();
    m_view.SetDPIColumnWidths();
    return CDocker::OnDPIChangedAfterParent(msg, wparam, lparam);
}

