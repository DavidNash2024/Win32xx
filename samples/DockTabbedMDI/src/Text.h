///////////////////////////////////////////////////////
// Text.h - Declaration of the CViewText, CContainText,
//          and CDockText classes
//

#ifndef TEXT_H
#define TEXT_H

#include <richedit.h>


///////////////////////////////////////////////////////////
// CViewText manages a rich edit control. It displays text.
// This is the view window used byCContainText.
class CViewText : public CRichEdit
{
public:
    CViewText();
    virtual ~CViewText();
    void OnEditCopy();
    void OnEditPaste();
    void OnEditCut();
    void OnEditDelete();
    void OnEditRedo();
    void OnEditUndo();

protected:
    virtual void OnAttach();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    void PreCreate(CREATESTRUCT& cs);

private:
    CFont m_font;
};


/////////////////////////////////////////////////
// CContainText manages a dock container. It uses
// CViewText as its view.
class CContainText : public CDockContainer
{
public:
    CContainText();
    ~CContainText() {}

private:
    CViewText m_viewText;
};


////////////////////////////////////////////////////
// CDockText manages a docker that uses CContainText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() {}

private:
    CContainText m_view;
};


#endif // TEXT_H

