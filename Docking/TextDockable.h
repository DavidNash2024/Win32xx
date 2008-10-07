/////////////////////////////////////////////
// TextDockable.h

#ifndef TEXTDOCKABLE_H
#define TEXTDOCKABLE_H

#include "../Win32++/dockable.h"


// Declaration of the CTextView class
class CTextView : public CWnd
{
public:
	CTextView();
	virtual ~CTextView(){}

protected:
	virtual void PreCreate(CREATESTRUCT &cs);

};


// Declaration of the CTextDockable class
class CTextDockable : public CDockable
{
public:
	CTextDockable();
	virtual ~CTextDockable() {}
	virtual void OnCreate();

private:
	CTextView m_View;

};


#endif // TEXTDOCKABLE_H

