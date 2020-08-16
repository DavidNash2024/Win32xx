/////////////////////////////
// SplitterPanes.cpp
//

#include "stdafx.h"
#include "SplitterPanes.h"

//////////////////////////////////
// CDockTree function definitions.
//
CDockText::CDockText()
{
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(6);
}

CDockTree::CDockTree()
{
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(12);
}

CDockList::CDockList()
{
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(6);
}

