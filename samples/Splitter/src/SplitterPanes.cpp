//////////////////////////////////////////////
// SplitterPanes.cpp

#include "stdafx.h"
#include "SplitterPanes.h"


CDockText::CDockText() 
{
    SetView(m_View);

    // Set the width of the splitter bar
    SetBarWidth(6);
}


CDockTree::CDockTree() 
{
    SetView(m_View);

    // Set the width of the splitter bar
    SetBarWidth(12);
}


CDockList::CDockList() 
{
    SetView(m_View);

    // Set the width of the splitter bar
    SetBarWidth(6);
}

