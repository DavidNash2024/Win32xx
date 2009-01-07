////////////////////////////////////////////
// Text.h - Declaration of the CViewText 
//          and CDockText classes

#ifndef TEXT_H
#define TEXT_H

#include "../../Win32++/dockable.h"
#include <richedit.h>


// Declaration of the CViewText class
class CViewText : public CWnd
{
public:
	CViewText();
	virtual ~CViewText();

protected:
	virtual void PreCreate(CREATESTRUCT &cs); 
	virtual void OnInitialUpdate(); 

private:
	HMODULE m_hRichEdit;

};

// Declaration of the CDockText class
class CDockText : public CDockable
{
public:
	CDockText();
	virtual ~CDockText() {}
	virtual void OnInitialUpdate();

private:
	CViewText m_View;

};


#endif // TEXT_H

