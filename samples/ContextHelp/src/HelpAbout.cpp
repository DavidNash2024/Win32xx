/////////////////////////////
// HelpAbout.cpp
//

#include "stdafx.h"
#include "HelpABout.h"
#include "resource.h"

//////////////////////////////////
// CAboutHelp function definitions
//

// Constructor.
CAboutHelp::CAboutHelp() : CDialog(IDW_ABOUT)
{
    m_credits = _T("Use the SetCredits function to specify this dialog's text");
}

// Called before the dialog is displayed.
BOOL CAboutHelp::OnInitDialog()
{
    SetDlgItemText(IDC_ABOUT_CREDITS, m_credits);
    return TRUE;
}
