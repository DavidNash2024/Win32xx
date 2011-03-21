////////////////////////////////////////////////////////
// SplitterPanes.cpp
//  Definitions for the CLeftPane and CRightPane classes

#include "stdafx.h"
#include "ExplorerApp.h"
#include "SplitterPanes.h"

void CRightPane::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(6);
}

