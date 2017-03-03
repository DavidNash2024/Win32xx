////////////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "PreviewDialog.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	CRect GetPrintRect();
	CRect GetPageRect();
	CRichEdit& GetRichView() { return m_RichView; }

	static  DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static  DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

	BOOL OnDropFiles(HDROP hDropInfo);
	BOOL OnEditCut();
	BOOL OnEditCopy();
	BOOL OnEditPaste();
	BOOL OnEditDelete();
	BOOL OnEditRedo();
	BOOL OnEditUndo();
	BOOL OnFileMRU(WPARAM wParam);
	BOOL OnFileExit();
	BOOL OnFileNew();
	BOOL OnFileOpen();
	BOOL OnFilePreview();
	BOOL OnFilePrint();
	BOOL OnFilePrintSetup();
	BOOL OnFileQuickPrint();
	BOOL OnFileSaveAs();
	BOOL OnFileSave();
	BOOL OnOptionsFont();
	BOOL OnOptionsWrap();

	void QuickPrint(CPrintDialog& PrintDlg);
	BOOL ReadFile(LPCTSTR szFileName);
	void SaveModifiedText();
	void SetPathName(LPCTSTR szFullFileName);
	void SetWindowTitle();
	BOOL WriteFile(LPCTSTR szFileName);

protected:
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupMenuIcons();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
private:
	CPreviewDialog	m_PrintPreview;
	CRichView m_RichView;
	CString m_PathName;
	BOOL m_IsWrapped;

};

#endif //MAINFRM_H

