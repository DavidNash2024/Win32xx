///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput, 
//          and CDockOutput classes

#ifndef OUTPUT_H
#define OUTPUT_H


#include <richedit.h>
#include "resource.h"

// Declaration of the CViewText class
class CViewOutput : public CRichEdit
{
public:
	CViewOutput();
	virtual ~CViewOutput();

protected:
	virtual void PreCreate(CREATESTRUCT& cs); 
	virtual void OnInitialUpdate(); 

};

// Declaration of the CContainText class
class CContainOutput : public CDockContainer
{
public:
	CContainOutput() 
	{
		SetView(m_ViewOutput); 
		SetDockCaption (_T("Output View - Docking container"));
		SetTabText(_T("Output"));
		SetTabIcon(IDI_TEXT);
	} 
	~CContainOutput() {}

private:
	CViewOutput m_ViewOutput;
};

// Declaration of the CDockText class
class CDockOutput : public CDocker
{
public:
	CDockOutput();
	virtual ~CDockOutput() {}

private:
	CContainOutput m_View;

};


#endif // OUTPUT_H

