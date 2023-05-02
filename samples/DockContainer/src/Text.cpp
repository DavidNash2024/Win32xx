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

// Called in response to a WM_DPICHANGED_AFTERPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_AFTERPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewText::OnDPIChangedAfterParent(UINT, WPARAM, LPARAM)
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
      case WM_DPICHANGED_AFTERPARENT: return OnDPIChangedAfterParent(msg, wparam, lparam);
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
    SetBarWidth(8);
}
