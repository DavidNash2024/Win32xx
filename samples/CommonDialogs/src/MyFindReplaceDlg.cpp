/////////////////////////////////////////
// MyFindReplaceDlg.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "MyFindReplaceDlg.h"

////////////////////////////////////////////
// MyFindReplaceDialog function definitions.
//

// This function is called before the dialog is displayed.
BOOL MyFindReplaceDialog::OnInitDialog()
{
    SetWindowText(m_boxTitle); return TRUE;
}

// Record the title of the find/replace dialog box after an object
// of this class is constructed, but before DoModal() is invoked.
void MyFindReplaceDialog::SetBoxTitle(LPCWSTR title)
{
    m_boxTitle = title;
}
