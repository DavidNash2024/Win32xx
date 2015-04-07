////////////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "RichView.h"


DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	BOOL ReadFile(LPCTSTR szFileName);
	void SetFileName(LPCTSTR szFullFileName);
	void SetWindowTitle();
	BOOL WriteFile(LPCTSTR szFileName);
	static  DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static  DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

protected:
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual void OnEditCut();
	virtual void OnEditCopy();
	virtual void OnEditPaste();
	virtual void OnEditDelete();
	virtual void OnEditRedo();
	virtual void OnEditUndo();
	virtual void OnFileMRU(WPARAM wParam);
	virtual void OnFileExit();
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual void OnFilePrint();
	virtual void OnFileSaveAs();
	virtual void OnFileSave();
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual void OnOptionsFont();
	virtual void OnOptionsWrap();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
private:
	CRichView m_RichView;
	CString m_strPathName;
	BOOL m_IsWrapped;
};

#endif //MAINFRM_H

