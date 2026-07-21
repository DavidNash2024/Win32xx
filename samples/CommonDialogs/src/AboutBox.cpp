/////////////////////////////////////////
// AboutBox.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "pch.h"
#include "AboutBox.h"
#include "resource.h"

/////////////////////////////////
// AboutBox function definitions.
//

// Called before the dialog is displayed to set the text in the AboutBox.
BOOL AboutBox::OnInitDialog()
{
    SetDlgItemText(IDC_ABOUT_MSG, m_about);
    return TRUE;
}

