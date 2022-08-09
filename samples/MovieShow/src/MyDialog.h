/////////////////////////////
// CViewDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "CoverImage.h"


////////////////////////////////////////////////////////////////
// CViewDialog manages the dialog which displays the information
// about the selected movie including the title, release date,
// cast, description and the cover image.
class CViewDialog : public CDialog
{
public:
    CViewDialog(UINT resID);
    virtual ~CViewDialog();

    const CRichEdit& GetActors() const { return m_actors; }
    const CRichEdit& GetInfo()   const { return m_info; }
    const CRichEdit& GetTitle()  const { return m_title; }
    const CRichEdit& GetYear()   const { return m_year; }

    CCoverImage& SetPicture() { return m_picture; }

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnCancel() {} // Suppress closing the dialog with esc key.
    virtual void    OnClose() {}  // Suppress closing the dialog with return key on rich edit.
    virtual BOOL    OnInitDialog();
    virtual void    OnOK() {}     // Suppress closing the dialog with return key.

private:
    CViewDialog(const CViewDialog&);                // Disable copy construction
    CViewDialog& operator = (const CViewDialog&);   // Disable assignment operator

    // Message handlers
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void AppendText(int nID, LPCTSTR text);

    CResizer    m_Resizer;
    CRichEdit   m_actors;
    CRichEdit   m_info;
    CRichEdit   m_title;
    CRichEdit   m_year;
    CCoverImage m_picture;
    CStatic     m_static1;
    CStatic     m_static2;
    CStatic     m_static3;
    CStatic     m_static4;
};


////////////////////////////////////////////////////////
// CDockDialog manages the resizing of the dialog window
// when the splitter bar is dragged.
class CDockDialog : public CDocker
{
public:
    CDockDialog();
    virtual ~CDockDialog() {}

    CViewDialog& GetViewDialog() { return m_view; }

private:
    CDockDialog(const CDockDialog&);                // Disable copy construction
    CDockDialog& operator = (const CDockDialog&);   // Disable assignment operator

    CViewDialog m_view;
};

#endif //MYDIALOG_H
