//////////////////////////
// RightPane.cpp
//

#include "stdafx.h"
#include "LeftPane.h"
#include "RightPane.h"

//////////////////////////////////
// CRightPane function definitions
//

// Constructor.
CRightPane::CRightPane()
{
    SetView(m_view);
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
