//////////////////////////////////////////////////////
// MDIChildText.h
//  Declaration of the CTextMDIChild class


#ifndef MDICHILDTEXT_H
#define MDICHILDTEXT_H

#include "../Win32++/mdi.h"

class CTextView : public CWnd
{
public:
	CTextView();
	virtual ~CTextView();
	virtual void OnCreate();
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	HFONT m_hFont;
	HMODULE   m_hRichEdit;
};




class CTextMDIChild : public CMDIChild
{
public:
	CTextMDIChild();
	virtual ~CTextMDIChild();

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTextView m_TextView;

};

#endif  //MDICHILDTEXT_H
