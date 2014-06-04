/////////////////////////////////////////////
// Dockers.cpp

#include "stdafx.h"
#include "DockingApp.h"
#include "Mainfrm.h"
#include "Dockers.h"


//////////////////////////////////////////////
//  Definitions for the CDockSimple class
CDockSimple::CDockSimple()
{
	// Set the view window for the simple docker
	SetView(m_View);
}

CDocker* CDockSimple::NewDockerFromID(int nID)
{
	CDocker* pDock = NULL;
	switch(nID)
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
	case ID_DOCK_SIMPLE1:
		pDock = new CDockSimple;
		break;
	case ID_DOCK_SIMPLE2:
		pDock = new CDockSimple;
		break;
	case ID_DOCK_TEXT1:
		pDock = new CDockText;
		break;
	case ID_DOCK_TEXT2:
		pDock = new CDockText;
		break;
	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDock;
}

void CDockSimple::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	SetCaption (_T("Simple View - Docking"));
}


//////////////////////////////////////////////
//  Definitions for the CDockText class
CDockText::CDockText()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockText::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	SetCaption (_T("Text View - Docking"));
}



//////////////////////////////////////////////
//  Definitions for the CDockClasses class
CDockClasses::CDockClasses()
{
	// Set the view window to our TreeView control
	SetView(m_View);
}

void CDockClasses::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	SetCaption (_T("Class View - Docking"));
}


//////////////////////////////////////////////
//  Definitions for the CDockFiles class
CDockFiles::CDockFiles()
{
	// Set the view window to our ListView control
	SetView(m_View);
}

void CDockFiles::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	SetCaption (_T("Files View - Docking"));
}
