//////////////////////////////////////////////
// MyTabbedMDI.cpp

#include "stdafx.h"
#include "MyTabbedMDI.h"
#include "Mainfrm.h"

CMyTabbedMDI::CMyTabbedMDI()
{
}

CWnd* CMyTabbedMDI::NewMDIChildFromID(int idMDIChild)
{
	CWnd* pView = NULL;
	switch(idMDIChild)
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
		pView = new CViewSimple;
		break;
	case ID_MDI_RECT:
		pView = new CViewRect;
		break;
	default:
		TRACE(_T("Unknown TabbedMDI id\n"));
		break;
	}

	return pView;
}

