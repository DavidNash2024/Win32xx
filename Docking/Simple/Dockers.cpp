/////////////////////////////////////////////
// TextDocker.cpp

#include "DockingApp.h"
#include "Mainfrm.h"
#include "Dockers.h"


//////////////////////////////////////////////
//  Definitions for the CDockSimple class
CDockSimple::CDockSimple()
{
	// Set the view window to our edit control
	SetView(m_View);
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
//  Definitions for the CDockClass class
CDockClass::CDockClass()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockClass::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	SetCaption (_T("Class View - Docking"));
}


//////////////////////////////////////////////
//  Definitions for the CDockFiles class
CDockFiles::CDockFiles()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockFiles::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	SetCaption (_T("Files View - Docking"));
}
