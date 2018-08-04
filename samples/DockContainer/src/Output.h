///////////////////////////////////////////////////////
// Output.h - Declaration of the CViewOutput, CContainOutput, 
//          and CDockOutput classes

#ifndef OUTPUT_H
#define OUTPUT_H


#include "resource.h"

// Declaration of the CViewText class
class CViewOutput : public CRichEdit
{
public:
    CViewOutput();
    virtual ~CViewOutput();

protected:
    virtual void OnAttach(); 
    virtual void PreCreate(CREATESTRUCT& cs);

};

// Declaration of the CContainText class
class CContainOutput : public CDockContainer
{
public:
    CContainOutput() 
    {
        SetView(m_viewOutput); 
        SetDockCaption (_T("Output View - Docking container"));
        SetTabText(_T("Output"));
        SetTabIcon(IDI_TEXT);
    } 
    ~CContainOutput() {}

private:
    CViewOutput m_viewOutput;
};

// Declaration of the CDockText class
class CDockOutput : public CDocker
{
public:
    CDockOutput();
    virtual ~CDockOutput() {}

private:
    CContainOutput m_view;

};


#endif // OUTPUT_H

