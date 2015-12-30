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

protected:
	virtual void PreCreate(CREATESTRUCT& cs); 
	virtual void OnInitialUpdate(); 

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

