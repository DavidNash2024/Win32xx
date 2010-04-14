//////////////////////////////////////////////
// MyTabbedMDI.cpp

#include "stdafx.h"
#include "MyTabbedMDI.h"
#include "Mainfrm.h"

CWnd* CMyTabbedMDI::NewMDIChildFromID(int nID)
{
	CWnd* pView = NULL;
	switch(nID)
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
	case ID_MDI_SIMPLE:
		pView = new CViewWeb;
		break;
	case ID_MDI_RECT:
		pView = new CViewRect;
		break;
	default:
		TRACE(_T("Unknown TabbedMDI ID\n"));
		break;
	}

	return pView;
}

