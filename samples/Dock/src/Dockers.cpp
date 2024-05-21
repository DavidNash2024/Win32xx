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

    // Set the width of the splitter bar.
    SetBarWidth(8);

    SetCaption (_T("Simple View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockSimple::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
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
    SetBarWidth(8);

    SetCaption(_T("Text View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockText::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
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
    SetBarWidth(8);

    SetCaption(_T("Class View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockClasses::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
       cs.dwExStyle |= WS_EX_COMPOSITED;
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
    SetBarWidth(8);

    SetCaption(_T("Files View - Docking"));
}

// Set the CREATESTURCT parameters before the window is created.
void CDockFiles::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CDocker::PreCreate(cs);

    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}
