///////////////////////////////////////////////////
// Output.cpp -  Definitions for the CViewOutput, CContainOutput
//              and CDockOutput classes


#include "stdafx.h"
#include "Output.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewOutput functions
CViewOutput::CViewOutput()
{
}

CViewOutput::~CViewOutput()
{
}

void CViewOutput::OnInitialUpdate()
{
	SetWindowText(_T("Output Window"));
}

void CViewOutput::PreCreate(CREATESTRUCT& cs)
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}


//////////////////////////////////////////////
//  Definitions for the CDockOutput class
CDockOutput::CDockOutput()
{
	// Set the view window to our edit control
	SetView(m_View);
}

int CDockOutput::OnCreate(CREATESTRUCT& cs)
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	return CDocker::OnCreate(cs);
}


