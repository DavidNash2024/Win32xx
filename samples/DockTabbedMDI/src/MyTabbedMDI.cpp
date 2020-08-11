/////////////////////////////
// MyTabbedMDI.cpp
//

#include "stdafx.h"
#include "MyTabbedMDI.h"
#include "Browser.h"
#include "Classes.h"
#include "Files.h"
#include "Output.h"
#include "Rect.h"
#include "Text.h"


////////////////////////////////////
// CMyTabbedMDI function definitions
//

// Constructor.
CMyTabbedMDI::CMyTabbedMDI()
{
}

// Adds a new MDI. The mdiChild parameter specifies the MDI type.
CWnd* CMyTabbedMDI::NewMDIChildFromID(int mdiChild)
{
    CWnd* pView = NULL;
    switch(mdiChild)
    {
    case ID_MDI_CLASSES:
        pView = new CViewClasses;
        break;
    case ID_MDI_FILES:
        pView = new CViewFiles;
        break;
    case ID_MDI_OUTPUT:
        pView = new CViewOutput;
        break;
    case ID_MDI_TEXT:
        pView = new CViewText;
        break;
    case ID_MDI_RECT:
        pView = new CViewRect;
        break;
    case ID_MDI_WEB:
        pView = new CViewWeb;
        break;
    default:
        TRACE("Unknown TabbedMDI id\n");
        break;
    }

    return pView;
}

