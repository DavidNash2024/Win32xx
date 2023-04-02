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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockSimple::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockText::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    m_view.SetDPIFont();
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockClasses::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    m_view.SetDPIImages();
    RecalcDockLayout();
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockFiles::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    m_view.SetDPIImages();
    m_view.SetDPIColumnWidths();
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
}
