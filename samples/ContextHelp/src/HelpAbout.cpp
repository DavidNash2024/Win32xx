
#include "stdafx.h"
#include "HelpABout.h"
#include "resource.h"


CHelpAbout::CHelpAbout() : CDialog(IDW_ABOUT) 
{
    m_Credits = _T("Use the SetCredits function to specify this dialog's text");
}

BOOL CHelpAbout::OnInitDialog()
{
    // do the base class initialization first
    CDialog::OnInitDialog();
    SetDlgItemText(IDC_ABOUT_CREDITS, m_Credits);
    return TRUE;
}
