////////////////////////////////////////////////////
// Mainfrm.h


#ifndef MAINFRM_H
#define MAINFRM_H


#include "../../Win32++/frame.h"
#include "RichView.h"


DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void OnDropFiles(HDROP hDropInfo);
	void OnEditCut();
	void OnEditCopy();
	void OnEditPaste();
	void OnEditDelete();
	void OnEditRedo();
	void OnEditUndo();
	void OnFileNew();
	void OnFileOpen();
	void OnFilePrint();
	void OnFileSaveAs();
	void OnFileSave();
	BOOL ReadFile(LPCTSTR szFileName);
	void SetFileName(TCHAR* szFullFileName);
	void SetWindowTitle();
	BOOL WriteFile(LPCTSTR szFileName);
	static  DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static  DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

protected:
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
private:
	CRichView m_RichView;
	tString m_stPathName;
};

#endif //MAINFRM_H

