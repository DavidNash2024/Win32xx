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
    CViewText() = default;
    virtual ~CViewText() override = default;

protected:
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewText(const CViewText&) = delete;
    CViewText& operator=(const CViewText&) = delete;

    CFont   m_font;
};


//////////////////////////////////////////////////////////////
// CMDIChildText manages a MDI child window. It uses CViewText
// as its view window.
class CMDIChildText : public CMDIChild
{
public:
    CMDIChildText();
    virtual ~CMDIChildText() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMDIChildText(const CMDIChildText&) = delete;
    CMDIChildText& operator=(const CMDIChildText&) = delete;

    CViewText m_textView;
    CMenu m_menu;
};

#endif  //MDICHILDTEXT_H
