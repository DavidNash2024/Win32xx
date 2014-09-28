////////////////////////////////////////////////////////////
// DockTabbedMDI.cpp -  Definitions for the CDockTabbedMDI class

#include "stdafx.h"
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

CDocker* CDockTabbedMDI::NewDockerFromID(int idDock)
{
	CDocker* pDock = NULL;
	switch(idDock)
	{
	case ID_DOCK_CLASSES1:
		pDock = new CDockClasses;
		break;
	case ID_DOCK_CLASSES2:
		pDock = new CDockClasses;					
		break;
	case ID_DOCK_FILES1:
		pDock = new CDockFiles;
		break;
	case ID_DOCK_FILES2:
		pDock = new CDockFiles;
		break;
	case ID_DOCK_OUTPUT1:
		pDock = new CDockOutput;
		break;
	case ID_DOCK_OUTPUT2:
		pDock = new CDockOutput;
		break;
	case ID_DOCK_TEXT1:
		pDock = new CDockText;
		break;
	case ID_DOCK_TEXT2:
		pDock = new CDockText;
		break;
	case ID_DOCK_DIALOG:
		pDock = new CDockDialog;
		break;
	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDock;
}

int CDockTabbedMDI::OnCreate(LPCREATESTRUCT pcs)
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	return CDocker::OnCreate(pcs);
}


