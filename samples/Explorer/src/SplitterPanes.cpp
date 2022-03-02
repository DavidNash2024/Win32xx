/////////////////////////////
// SplitterPanes.cpp
//

#include "stdafx.h"
#include "SplitterPanes.h"

////////////////////////////////////////////////
// CLeftPane and CRightPane function definitions
//

// Constructor.
CLeftPane::CLeftPane()
{
    SetView(m_view);
}

// Constructor.
CRightPane::CRightPane()
{
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(6);
}

// Adds a new docker. The id specifies the dock type.
// The id is used by LoadDockRegistrySettings to restore the
// previous splitter window arrangement.
CDocker* CRightPane::NewDockerFromID(int id)
{
    CDocker* pDock = NULL;
    switch (id)
    {
    case ID_DOCK_LEFTPANE:
        pDock = new CLeftPane;
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return pDock;
}
