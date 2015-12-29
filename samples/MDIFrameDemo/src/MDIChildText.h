//////////////////////////////////////////////////////////
// MDIChildText.h
//  Declaration of the CViewText and CMDIChildText classes

#ifndef MDICHILDTEXT_H
#define MDICHILDTEXT_H


class CViewText : public CRichEdit
{
public:
	CViewText();
	virtual ~CViewText();
	virtual void OnAttach();

private:
	CFont	m_Font;
};


class CMDIChildText : public CMDIChild
{
public:
	CMDIChildText();
	virtual ~CMDIChildText();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CViewText m_TextView;
	CMenu m_Menu;
};

#endif  //MDICHILDTEXT_H
