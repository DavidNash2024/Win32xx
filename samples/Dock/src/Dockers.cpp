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
    SetBarWidth(DPIScaleInt(8));

    SetCaption (_T("Simple View - Docking"));
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

    SetCaption (_T("Text View - Docking"));
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

    SetCaption (_T("Class View - Docking"));
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

    SetCaption (_T("Files View - Docking"));
}

