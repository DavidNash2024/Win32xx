////////////////////////////////////////////////////////////
// TabbedMDI.cpp -  Definitions for the CDockTabbedMDI class


#include "TabbedMDIApp.h"
#include "DockTabbedMDI.h"
#include "resource.h"


//////////////////////////////////////////////
//  Definitions for the CDockTabbedMDI class
CDockTabbedMDI::CDockTabbedMDI()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockTabbedMDI::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}


