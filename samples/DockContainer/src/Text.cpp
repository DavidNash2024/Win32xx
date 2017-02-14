///////////////////////////////////////////////////
// Text.cpp -  Definitions for the CViewText, 
//              and CDockText classes

#include "stdafx.h"
#include "Text.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewText functions
CViewText::CViewText()
{
}

CViewText::~CViewText()
{
}

void CViewText::OnAttach()
{
	SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));
}

void CViewText::PreCreate(CREATESTRUCT& cs)
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}


//////////////////////////////////////////////
//  Definitions for the CContainText class
CContainText::CContainText() 
{
	SetDockCaption (_T("Text View - Docking container"));
	SetTabText(_T("Text"));
	SetTabIcon(IDI_TEXT);
	SetView(m_ViewText);
} 


//////////////////////////////////////////////
//  Definitions for the CDockText class
CDockText::CDockText()
{
	// Set the view window to our edit control
	SetView(m_View);

	// Set the width of the splitter bar
	SetBarWidth(8);
}

