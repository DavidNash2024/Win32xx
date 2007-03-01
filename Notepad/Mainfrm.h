////////////////////////////////////////////////////
// Mainfrm.h


#ifndef MAINFRM_H
#define MAINFRM_H


#include <string>
#include "..\Win32++\frame.h"
#include "RichView.h"

DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnClose();
	virtual BOOL OnCommand(UINT nID);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual void OnEditCut();
	virtual void OnEditCopy();
	virtual void OnEditPaste();
	virtual void OnEditDelete();
	virtual void OnEditRedo();
	virtual void OnEditUndo();
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual void OnFilePrint();
	virtual void OnFileSaveAs();
	virtual void OnFileSave();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void ReadFile(LPCTSTR szFileName);
	virtual void SetFileName(TCHAR* szFullFileName);
	virtual void SetWindowTitle();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void WriteFile(LPCTSTR szFileName);
	static DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
	static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

private:
	CRichView m_RichView;
	std::basic_string<TCHAR> m_strPathName;
};

#endif //MAINFRM_H

