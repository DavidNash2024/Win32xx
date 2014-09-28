////////////////////////////////////////////////////////
// SplitterPanes.cpp
//  Definitions for the CLeftPane and CRightPane classes

#include "stdafx.h"
#include "ExplorerApp.h"
#include "SplitterPanes.h"

int CRightPane::OnCreate(LPCREATESTRUCT pcs)
{
	// Set the width of the splitter bar
	SetBarWidth(6);

	return CDocker::OnCreate(pcs);
}

