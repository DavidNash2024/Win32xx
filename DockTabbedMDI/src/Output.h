///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput, 
//          and CDockOutput classes

#ifndef OUTPUT_H
#define OUTPUT_H


#include <richedit.h>


// Declaration of the CViewText class
class CViewOutput : public CWnd
{
public:
	CViewOutput();
	virtual ~CViewOutput();

protected:
	virtual void PreCreate(CREATESTRUCT &cs); 
	virtual void OnInitialUpdate(); 

private:
	HMODULE m_hRichEdit;

};

// Declaration of the CContainText class
class CContainOutput : public CContainer
{
public:
	CContainOutput();
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
	virtual void OnInitialUpdate();

private:
	CContainOutput m_View;

};


#endif // OUTPUT_H

