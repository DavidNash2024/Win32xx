/////////////////////////////////////////////
// Dockers.cpp

#include "stdafx.h"
#include "Dockers.h"


//////////////////////////////////////////////
//  Definitions for the CDockSimple class
CDockSimple::CDockSimple()
{
    // Set the view window for the simple docker
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);

    SetCaption (_T("Simple View - Docking"));
}


//////////////////////////////////////////////
//  Definitions for the CDockText class
CDockText::CDockText()
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);

    SetCaption (_T("Text View - Docking"));
}


//////////////////////////////////////////////
//  Definitions for the CDockClasses class
CDockClasses::CDockClasses()
{
    // Set the view window to our TreeView control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);

    SetCaption (_T("Class View - Docking"));
}


//////////////////////////////////////////////
//  Definitions for the CDockFiles class
CDockFiles::CDockFiles()
{
    // Set the view window to our ListView control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);

    SetCaption (_T("Files View - Docking"));
}


