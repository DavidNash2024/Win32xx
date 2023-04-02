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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockText::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(4));
    m_view.SetDPIFont();
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockTree::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    m_view.SetDPIImages();
    RecalcDockLayout();
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockList::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(4));
    m_view.SetDPIImages();
    m_view.SetDPIColumnWidths();
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
}

