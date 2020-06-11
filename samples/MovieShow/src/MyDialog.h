///////////////////////////////////////
// CViewDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "CoverImage.h"

////////////////////////////////////////////////////////////
// Declaration of the CViewDialog class
// The dialog displays information about the selected movie
//  including the title, release date, cast, description and
//  the cover image.
class CViewDialog : public CDialog
{
public:
    CViewDialog(UINT resID);
    virtual ~CViewDialog();

protected:
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel() {}
    virtual void OnClose() {}
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnOK() {}

//private:
public:
    void AppendText(int nID, LPCTSTR text);

    CResizer m_Resizer;

    CStatic     m_static1;
    CStatic     m_static2;
    CStatic     m_static3;
    CStatic     m_static4;
    CCoverImage m_picture;

    CRichEdit   m_title;
    CRichEdit   m_year;
    CRichEdit   m_actors;
    CRichEdit   m_description;

    CFont       m_textFont;
    CFont       m_titleFont;
};

////////////////////////////////////////////////////////////
// Declaration of the CDockDialog class
// Manages the resizing of the dialog window when the 
//  splitter bar is dragged.
class CDockDialog : public CDocker
{
public:
    CDockDialog();
    virtual ~CDockDialog() {}

private:
    CViewDialog m_view;
};

#endif //MYDIALOG_H
