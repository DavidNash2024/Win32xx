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

CDocker* CDockTabbedMDI::NewDockerFromID(int nID)
{
	CDocker* pDock = NULL;
	switch(nID)
	{
	case ID_CLASSES1:
		pDock = new CDockClasses;
		break;
	case ID_CLASSES2:
		pDock = new CDockClasses;					
		break;
	case ID_FILES1:
		pDock = new CDockFiles;
		break;
	case ID_FILES2:
		pDock = new CDockFiles;
		break;
	case ID_OUTPUT1:
		pDock = new CDockOutput;
		break;
	case ID_OUTPUT2:
		pDock = new CDockOutput;
		break;
	case ID_TEXT1:
		pDock = new CDockText;
		break;
	case ID_TEXT2:
		pDock = new CDockText;
		break;
	default:
		TRACE(_T("Unknown Dock ID\n"));
		break;
	}

	return pDock;
}



void CDockTabbedMDI::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}


