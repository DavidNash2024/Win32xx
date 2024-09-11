///////////////////////////////////////////////////////
// Text.h - Declaration of the CViewText, CContainText,
//          and CDockText classes
//

#ifndef TEXT_H
#define TEXT_H


////////////////////////////////////////////////////////////
// CViewText manages an rich edit control. It displays text.
// This is the view window for CContainText.
class CViewText : public CRichEdit
{
public:
    CViewText();
    virtual ~CViewText() override;
    void SetDPIFont();

protected:
    // Virtual functions that override base class functions.
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewText(const CViewText&) = delete;
    CViewText& operator=(const CViewText&) = delete;

    CFont m_font;
};


/////////////////////////////////////////////////
// CContainText manages a dock container. It uses
// CViewText as its view.
class CContainText : public CDockContainer
{
public:
    CContainText();
    virtual ~CContainText() override {}

protected:
    // Virtual functions that override base class functions.
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CContainText(const CContainText&) = delete;
    CContainText& operator=(const CContainText&) = delete;

    CViewText m_viewText;
};


///////////////////////////////////////////////////
// CDockText manages a docker. It uses CContainText
// as its view.
class CDockText : public CDocker
{
public:
    CDockText();
    virtual ~CDockText() override {}

protected:
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockText(const CDockText&) = delete;
    CDockText& operator=(const CDockText&) = delete;

    CContainText m_view;
};


#endif // TEXT_H
