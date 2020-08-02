////////////////////////////////////////////////////////
// SplitterPanes.cpp
//  Definitions for the CLeftPane and CRightPane classes

#include "stdafx.h"
#include "SplitterPanes.h"


CLeftPane::CLeftPane()
{
    SetView(m_view);
}

CRightPane::CRightPane()
{
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(6);
}

