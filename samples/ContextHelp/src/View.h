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
    virtual ~CView() override = default;
    virtual HWND Create(HWND hParent) override;

    void DpiScaleImage();
    CDoc& GetDoc();

    // Command handlers
    BOOL OnButton();
    BOOL OnCheckA();
    BOOL OnCheckB();
    BOOL OnCheckC();
    BOOL OnRangeOfIDs(UINT firstID, UINT lastID, UINT clickedID);

    // Command handlers
    INT_PTR OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

protected:
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void OnCancel() override;
    virtual void OnClose() override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    // Member variables
    CDoc     m_doc;
    CResizer m_resizer;
    HWND     m_parent;      // handle of parent frame
    CBitmap  m_patternImage;

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
