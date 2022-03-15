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

private:
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
    CViewText m_textView;
    CMenu m_menu;
};

#endif  //MDICHILDTEXT_H
