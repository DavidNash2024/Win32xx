
#include "stdafx.h"
#include "HelpABout.h"
#include "resource.h"


CAboutHelp::CAboutHelp() : CDialog(IDW_ABOUT)
{
    m_credits = _T("Use the SetCredits function to specify this dialog's text");
}

BOOL CAboutHelp::OnInitDialog()
{
    // do the base class initialization first
    CDialog::OnInitDialog();
    SetDlgItemText(IDC_ABOUT_CREDITS, m_credits);
    return TRUE;
}
