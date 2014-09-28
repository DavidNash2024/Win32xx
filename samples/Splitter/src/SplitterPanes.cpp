//////////////////////////////////////////////
// SplitterPanes.cpp

#include "stdafx.h"
#include "SplitterApp.h"
#include "Mainfrm.h"
#include "SplitterPanes.h"


CDockSimple::CDockSimple() 
{
	SetView(m_View);
}


CDockText::CDockText() 
{
	SetView(m_View);
}

int CDockText::OnCreate(LPCREATESTRUCT pcs)
{
	// Set the width of the splitter bar
	SetBarWidth(6);

	return CDocker::OnCreate(pcs);
}


CDockTree::CDockTree() 
{
	SetView(m_View);
}

int CDockTree::OnCreate(LPCREATESTRUCT pcs) 
{
	// Set the width of the splitter bar
	SetBarWidth(12);

	return CDocker::OnCreate(pcs);
}


CDockList::CDockList() 
{
	SetView(m_View);
}

int CDockList::OnCreate(LPCREATESTRUCT pcs)
{
	// Set the width of the splitter bar
	SetBarWidth(6);

	return CDocker::OnCreate(pcs);
}

