//////////////////////////////////////////////////////////
// MDIChildText.h
//  Declaration of the CViewText and CMDIChildText classes

#ifndef MDICHILDTEXT_H
#define MDICHILDTEXT_H


///////////////////////////////////////////////////////
// CViewText manages a rich edit control. It is used as
// the view window for CMDIChildText.
class CViewText : public CRichEdit
{
public:
    CViewText();
    virtual ~CViewText();

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CViewText(const CViewText&);                // Disable copy construction
    CViewText& operator = (const CViewText&);   // Disable assignment operator

    CFont   m_font;
};


//////////////////////////////////////////////////////////////
// CMDIChildText manages a MDI child window. It uses CViewText
// as its view window.
class CMDIChildText : public CMDIChild
{
public:
    CMDIChildText();
    virtual ~CMDIChildText();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CMDIChildText(const CMDIChildText&);                // Disable copy construction
    CMDIChildText& operator = (const CMDIChildText&);   // Disable assignment operator

    CViewText m_textView;
    CMenu m_menu;
};

#endif  //MDICHILDTEXT_H
