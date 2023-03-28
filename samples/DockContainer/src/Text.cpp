///////////////////////////////////////////////////
// Text.cpp -  Definitions for the CViewText,
//              and CDockText classes
//

#include "stdafx.h"
#include "Text.h"
#include "resource.h"


/////////////////////////////////
// CViewText function definitions
//

// Constructor.
CViewText::CViewText()
{
}

// Destructor.
CViewText::~CViewText()
{
}

// Called when a window handle (HWND) is attached to CViewText.
void CViewText::OnAttach()
{
    SetDPIFont();
    SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));
}

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewText::OnUserDPIChanged(UINT, WPARAM, LPARAM)
{
    SetDPIFont();
    return 0;
}

// Adjusts the font size in response to window DPI changes.
void CViewText::SetDPIFont()
{
    m_font.CreatePointFont(100, _T("Courier New"));
    m_font = DPIScaleFont(m_font, 9);
    SetFont(m_font);
}

// Process the window messages sent to this message.
LRESULT CViewText::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_DPICHANGED:    return OnUserDPIChanged(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}


////////////////////////////////////
// CContainText function definitions
//

// Constructor.
CContainText::CContainText()
{
    SetDockCaption(_T("Text View - Docking container"));
    SetTabText(_T("Text"));
    SetTabIcon(IDI_TEXT);
    SetView(m_viewText);
}


/////////////////////////////////
// CDockText function definitions
//

// Constructor.
CDockText::CDockText()
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(DPIScaleInt(8));
}

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockText::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
}

