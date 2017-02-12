///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput, 
//          and CDockOutput classes

#ifndef OUTPUT_H
#define OUTPUT_H


#include <richedit.h>


// Declaration of the CViewText class
class CViewOutput : public CRichEdit
{
public:
	CViewOutput();
	virtual ~CViewOutput();

protected:
	virtual void OnAttach();
	virtual void PreCreate(CREATESTRUCT& cs); 

private:
	CFont m_Font;
};

// Declaration of the CContainText class
class CContainOutput : public CDockContainer
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

private:
	CContainOutput m_View;

};


#endif // OUTPUT_H

