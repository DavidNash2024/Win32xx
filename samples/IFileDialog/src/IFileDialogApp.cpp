/////////////////////////////
// IFileDialogApp.cpp
//

#include "stdafx.h"
#include "IFileDialogApp.h"
#include "resource.h"


///////////////////////////////////////
// CIFileDialogApp function definitions
//

// Called when the application starts.
BOOL CIFileDialogApp::InitInstance()
{
    // Display the main taskdialg.
    myTaskDialog.DoModal();

    // Ask the application to quit.
    ::PostQuitMessage(0);
    return TRUE;
}
