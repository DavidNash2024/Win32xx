//////////////////////////////////////////////////////////
// MDIChildText.h
//  Declaration of the CViewText and CMDIChildText classes

#ifndef MDICHILDTEXT_H
#define MDICHILDTEXT_H


class CViewText : public CRichEdit
{
public:
    CViewText();
    virtual ~CViewText();
    virtual void OnAttach();

private:
    CFont   m_font;
};


class CMDIChildText : public CMDIChild
{
public:
    CMDIChildText();
    virtual ~CMDIChildText();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewText m_textView;
    CMenu m_menu;
};

#endif  //MDICHILDTEXT_H
