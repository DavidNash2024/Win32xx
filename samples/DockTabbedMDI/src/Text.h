///////////////////////////////////////////////////////
// Text.h - Declaration of the CViewText, CContainText,
//          and CDockText classes
//

#ifndef TEXT_H
#define TEXT_H

#include <richedit.h>


///////////////////////////////////////////////////////////
// CViewText manages a rich edit control. It displays text.
// This is the view window used by CContainText.
class CViewText : public CRichEdit
{
public:
    CViewText() = default;
    virtual ~CViewText() override = default;

protected:
    virtual void OnAttach() override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewText(const CViewText&) = delete;
    CViewText& operator=(const CViewText&) = delete;

    // Command handlers
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditCut();
    BOOL OnEditDelete();
    BOOL OnEditRedo();
    BOOL OnEditUndo();
};


/////////////////////////////////////////////////
// CContainText manages a dock container. It uses
// CViewText as its view.
class CContainText : public CDockContainer
{
public:
    CContainText();
    virtual ~CContainText() override = default;

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CContainText(const CContainText&) = delete;
    CContainText& operator=(const CContainText&) = delete;

    CViewText m_viewText;
};


////////////////////////////////////////////////////
// CDockText manages a docker that uses CContainText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() override = default;

protected:
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockText(const CDockText&) = delete;
    CDockText& operator=(const CDockText&) = delete;

    CContainText m_view;
};


#endif // TEXT_H

