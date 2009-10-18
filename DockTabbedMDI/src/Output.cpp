///////////////////////////////////////////////////
// Text.cpp -  Definitions for the CViewOutput, CContainOutput
//              and CDockOutput classes


#include "stdafx.h"
#include "TabbedMDIApp.h"
#include "Output.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewOutput functions
CViewOutput::CViewOutput()
{
	m_hRichEdit = ::LoadLibrary(_T("Riched20.dll")); // RichEdit ver 2.0
    if (!m_hRichEdit)
    {
		::MessageBox(NULL,_T("CRichView::CRichView  Failed to load Riched20.dll"), _T(""), MB_ICONWARNING);
    }
}

CViewOutput::~CViewOutput(void)
{
	if (m_hRichEdit)
		::FreeLibrary(m_hRichEdit);
}

void CViewOutput::OnInitialUpdate()
{
	SetWindowText(_T("Output Window"));
}

void CViewOutput::PreCreate(CREATESTRUCT &cs)
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

	cs.lpszClass = RICHEDIT_CLASS; // RichEdit ver 2.0
}


//////////////////////////////////////////////
//  Definitions for the CContainOutput class
CContainOutput::CContainOutput() 
{
	SetView(m_ViewOutput); 
	SetDockCaption (_T("Output View - Docking container"));
	SetTabText(_T("Output"));
	SetTabIcon(IDI_TEXT);
} 


//////////////////////////////////////////////
//  Definitions for the CDockOutput class
CDockOutput::CDockOutput()
{
	// Set the view window to our edit control
	SetView(m_View);
}

void CDockOutput::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}


