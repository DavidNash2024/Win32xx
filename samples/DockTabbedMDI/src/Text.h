///////////////////////////////////////////////////////
// Text.h - Declaration of the CViewText, CContainText, 
//          and CDockText classes

#ifndef TEXT_H
#define TEXT_H


#include <richedit.h>


// Declaration of the CViewText class
class CViewText : public CWnd
{
public:
	CViewText();
	virtual ~CViewText();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs); 

private:
	HMODULE m_hRichEdit;

};

// Declaration of the CContainText class
class CContainText : public CDockContainer
{
public:
	CContainText();
	~CContainText() {}

private:
	CViewText m_ViewText;
};

// Declaration of the CDockText class
class CDockText : public CDocker
{
public:
	CDockText();
	virtual ~CDockText() {}
	virtual void OnInitialUpdate();

private:
	CContainText m_View;

};


#endif // TEXT_H

