//////////////////////////////////////////////
// MDIChildText.cpp
//  Definitions for the CMDIChildText class

#include "stdafx.h"
#include "MDIChildText.h"
#include "resource.h"


CViewText::CViewText()
{
}

CViewText::~CViewText()
{
}

void CViewText::OnAttach()
{
    //Set font
    if (!m_Font)
    {
        m_Font.CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
    }

    SendMessage(WM_SETFONT, (WPARAM)m_Font.GetHandle(), 0L);
}

CMDIChildText::CMDIChildText()
{
    m_Menu.LoadMenu(_T("MdiMenuText"));
    SetHandles(m_Menu, NULL);
    SetView(m_TextView);
}

CMDIChildText::~CMDIChildText()
{   
}

BOOL CMDIChildText::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch(nID)
    {
    case IDM_EDIT_COPY:
        GetView().SendMessage(WM_COPY, 0, 0);
        return TRUE;
    case IDM_EDIT_PASTE:
        GetView().SendMessage(WM_PASTE, 0, 0);
        return TRUE;
    }

    return FALSE;
}

int CMDIChildText::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(_T("Text Window"));
    SetIconLarge(IDI_TEXT);
    SetIconSmall(IDI_TEXT);
    
    return CMDIChild::OnCreate(cs);
}


