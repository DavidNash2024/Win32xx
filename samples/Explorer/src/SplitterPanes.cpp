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

