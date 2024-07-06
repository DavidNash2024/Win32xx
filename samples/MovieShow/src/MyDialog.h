/////////////////////////////
// CViewDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "CoverImage.h"


////////////////////////////////////////////////////////////////
// CViewDialog manages the dialog that displays the information
// about the selected movie including the title, release date,
// cast, description and the cover image.
class CViewDialog : public CDialog
{
public:
    CViewDialog(UINT resID);
    virtual ~CViewDialog() override;
    CViewDialog(const CViewDialog&) = delete;               // Disable copy construction
    CViewDialog& operator=(const CViewDialog&) = delete;    // Disable assignment operator

    const CRichEdit& GetActors() const { return m_actors; }
    const CRichEdit& GetInfo()   const { return m_info; }
    const CRichEdit& GetTitle()  const { return m_title; }
    const CRichEdit& GetYear()   const { return m_year; }

    void SetDialogFonts();
    CCoverImage& SetPicture() { return m_picture; }

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    OnCancel() override {} // Suppress closing the dialog with esc key.
    virtual void    OnClose() override {}  // Suppress closing the dialog with return key on rich edit.
    virtual BOOL    OnInitDialog() override;
    virtual void    OnOK() override {}     // Suppress closing the dialog with return key.

private:
    // Message handlers
    virtual LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
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
    virtual ~CDockDialog() override {}
    CDockDialog(const CDockDialog&) = delete;              // Disable copy construction
    CDockDialog& operator=(const CDockDialog&) = delete;   // Disable assignment operator

    CViewDialog& GetViewDialog() { return m_view; }

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewDialog m_view;
};

#endif //MYDIALOG_H
