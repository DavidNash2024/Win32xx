/////////////////////////////
// LeftPane.cpp
//

#include "stdafx.h"
#include "LeftPane.h"

/////////////////////////////////
// CLeftPane function definitions
//

// Constructor.
CLeftPane::CLeftPane()
{
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(DPIScaleInt(6));
}

