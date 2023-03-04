/////////////////////////////
// SplitterPanes.cpp
//

#include "stdafx.h"
#include "SplitterPanes.h"

//////////////////////////////////
// CDockTree function definitions.
//

// CDocktext constructor.
CDockText::CDockText()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(6));
}

// CDockTree constructor.
CDockTree::CDockTree()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(12));
}

// CDockList constructor.
CDockList::CDockList()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(6));
}

