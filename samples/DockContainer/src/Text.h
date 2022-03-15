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
    virtual ~CViewText();

protected:
    // Virtual functions that override base class functions
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void OnAttach();
};


/////////////////////////////////////////////////
// CContainText manages a dock container. It uses
// CViewText as its view.
class CContainText : public CDockContainer
{
public:
    CContainText();
    virtual ~CContainText() {}

private:
    CViewText m_viewText;
};


///////////////////////////////////////////////////
// CDockText manages a docker. It uses CContainText
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

