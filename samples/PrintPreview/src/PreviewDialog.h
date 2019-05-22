///////////////////////////////////////
// PreviewDialog.h

#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H


////////////////////////////////////////
// Declaration of the CPreviewPane class
// The PreviewPane is a custom control within the PreviewDialog
//
class CPreviewPane : public CWnd
{
public:
    CPreviewPane();
    virtual ~CPreviewPane() {}

    void Render(CDC& dc);
    void SetBitmap(CBitmap& bitmap) { m_bitmap = bitmap; }

protected:
    virtual void OnDraw(CDC& dc);
    virtual BOOL OnEraseBkgnd(CDC&);
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CBitmap m_bitmap;

};


//////////////////////////////////////////
// Declaration of the CPrintPreview class
//
class CPrintPreview : public CDialog
{
public:
    CPrintPreview(UINT resID);
    virtual ~CPrintPreview();

    CPreviewPane& GetPreviewPane()  { return m_previewPane; }
    CRichEdit& GetRichView();
    BOOL DoPrintPreview(RECT pageRect, RECT printArea);
    BOOL OnPrintButton();
    BOOL OnNextButton();
    BOOL OnPrevButton();
    BOOL OnCloseButton();
    void PreviewPage(UINT page);
    void UpdateButtons();

protected:
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);

private:
    CResizer m_resizer;
    CPreviewPane m_previewPane;
    CButton m_buttonPrint;
    CButton m_buttonNext;
    CButton m_buttonPrev;
    CButton m_buttonClose;

    UINT    m_currentPage;
    CRect   m_pageRect;
    CRect   m_printArea;

public:
    std::vector<int> m_pageBreaks;

};

#endif //PREVIEWDIALOG_H
