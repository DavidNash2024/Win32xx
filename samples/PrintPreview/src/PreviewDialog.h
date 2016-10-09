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
	void SetBitmap(CBitmap& Bitmap) { m_Bitmap = Bitmap; }

protected:
	virtual void OnDraw(CDC& dc);
	virtual BOOL OnEraseBkgnd(CDC&);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CBitmap m_Bitmap;

};


//////////////////////////////////////////
// Declaration of the CPreviewDialog class
//
class CPreviewDialog : public CDialog
{
public:
	CPreviewDialog(UINT nResID);
	virtual ~CPreviewDialog();

	CPreviewPane& GetPreviewPane()  { return m_PreviewPane; }
	CRichEdit& GetRichView();
	BOOL DoPrintPreview(RECT rcPage, RECT rcPrintArea);
	BOOL OnPrintButton();
	BOOL OnNextButton();
	BOOL OnPrevButton();
	BOOL OnCloseButton();
	void PreviewPage(UINT nPage);
	void UpdateButtons();

protected:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	CResizer m_Resizer;
	CPreviewPane m_PreviewPane;
	CButton m_ButtonPrint;
	CButton m_ButtonNext;
	CButton m_ButtonPrev;
	CButton m_ButtonClose;

	UINT	m_CurrentPage;
	CRect	m_rcPage;
	CRect	m_rcPrintArea;

public:
	std::vector<int> m_PageBreaks;

};

#endif //PREVIEWDIALOG_H
