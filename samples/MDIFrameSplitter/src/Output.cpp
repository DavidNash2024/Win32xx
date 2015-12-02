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
	m_hRichEdit = ::LoadLibrary(_T("Riched20.dll")); // RichEdit ver 2.0
    if (m_hRichEdit == 0)
    {
		::MessageBox(NULL,_T("CRichView::CRichView  Failed to load Riched20.dll"), _T(""), MB_ICONWARNING);
    }
}

CViewOutput::~CViewOutput()
{
	if (m_hRichEdit)
		::FreeLibrary(m_hRichEdit);
}

void CViewOutput::OnInitialUpdate()
{
	SetWindowText(_T("Output Window"));
}

void CViewOutput::PreCreate(CREATESTRUCT& cs)
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

	cs.lpszClass = RICHEDIT_CLASS; // RichEdit ver 2.0
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


