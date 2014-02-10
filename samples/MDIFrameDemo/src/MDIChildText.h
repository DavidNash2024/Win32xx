//////////////////////////////////////////////////////////
// MDIChildText.h
//  Declaration of the CViewText and CMDIChildText classes

#ifndef MDICHILDTEXT_H
#define MDICHILDTEXT_H


class CViewText : public CWnd
{
public:
	CViewText();
	virtual ~CViewText();
	virtual void OnCreate();
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	CFont	m_Font;
	HMODULE m_hRichEdit;
};


class CMDIChildText : public CMDIChild
{
public:
	CMDIChildText();
	virtual ~CMDIChildText();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CViewText m_TextView;

};

#endif  //MDICHILDTEXT_H
