////////////////////////////////////////////////////////
// SplitterPanes.cpp
//  Definitions for the CLeftPane and CRightPane classes

#include "stdafx.h"
#include "SplitterPanes.h"


CLeftPane::CLeftPane()
{ 
    SetView(m_View);
}

CRightPane::CRightPane()
{
    SetView(m_View);

    // Set the width of the splitter bar
    SetBarWidth(6);
}

