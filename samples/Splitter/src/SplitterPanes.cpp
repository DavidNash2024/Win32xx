/////////////////////////////
// SplitterPanes.cpp
//

#include "stdafx.h"
#include "SplitterPanes.h"

//////////////////////////////////
// CDockText function definitions.
//

// CDocktext constructor.
CDockText::CDockText()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(4);
}


//////////////////////////////////
// CDockTree function definitions.
//

// CDockTree constructor.
CDockTree::CDockTree()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(8);
}


//////////////////////////////////
// CDockList function definitions.
//

// CDockList constructor.
CDockList::CDockList()
{
    SetView(m_view);

    // Set the width of the splitter bar.
    SetBarWidth(4);
}
