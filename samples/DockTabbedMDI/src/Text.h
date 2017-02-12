///////////////////////////////////////////////////////
// Text.h - Declaration of the CViewText, CContainText, 
//          and CDockText classes

#ifndef TEXT_H
#define TEXT_H


#include <richedit.h>


// Declaration of the CViewText class
class CViewText : public CRichEdit
{
public:
	CViewText();
	virtual ~CViewText();
	void OnEditCopy();
	void OnEditPaste();
	void OnEditCut();
	void OnEditDelete();
	void OnEditRedo();
	void OnEditUndo();

protected:
	virtual void OnAttach();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void PreCreate(CREATESTRUCT& cs);

private:
	CFont m_Font;
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

private:
	CContainText m_View;

};


#endif // TEXT_H

