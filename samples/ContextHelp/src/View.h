/////////////////////////////
// View.h
//

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "Doc.h"

//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CDialog
{
public:
    CView(UINT resID);
    virtual ~CView();
    virtual HWND Create(HWND hParent);

    CDoc& GetDoc();

    // Command handlers
    BOOL OnButton();
    BOOL OnCheckA();
    BOOL OnCheckB();
    BOOL OnCheckC();
    BOOL OnRangeOfIDs(UINT firstID, UINT lastID, UINT clickedID);

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel();
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    // Member variables
    CDoc     m_doc;
    CResizer m_resizer;
    HWND     m_parent;      // handle of parent frame

    // Member variables for dialog controls.
    CButton   m_radioA;
    CButton   m_radioB;
    CButton   m_radioC;
    CButton   m_checkA;
    CButton   m_checkB;
    CButton   m_checkC;
    CButton   m_button;
    CListBox  m_listBox;
    CEdit     m_edit;
    CRichEdit m_richEdit;
    CStatic   m_picture;
    CStatic   m_status;
    CButton   m_ok;
    CWnd      m_group;
};

#endif //SDI_VIEW_H
