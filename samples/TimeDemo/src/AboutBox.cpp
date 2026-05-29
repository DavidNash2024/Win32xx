/////////////////////////////////////////
// AboutBox.cpp
// Authors: Robert Tausworthe, David Nash
//

#include "stdafx.h"
#include "AboutBox.h"
#include "resource.h"


/////////////////////////////////
// AboutBox function definitions.
//

// AboutBox constructor. It uses the IDW_ABOUT string specified in resource.rc.
AboutBox::AboutBox() : CDialog(IDW_ABOUT)
{
}

// Call when the AboutBox dialog is created.
BOOL AboutBox::OnInitDialog()
{
     SetDlgItemText(IDC_CREDITS, m_status);
     return TRUE;
}
