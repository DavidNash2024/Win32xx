//////////////////////////////////////////////
// RichView.h

#ifndef RICHVIEW_H
#define RICHVIEW_H


class CRichView : public CRichEdit
{
public:
    CRichView();
    virtual ~CRichView();
    virtual void SetFontDefaults();

protected:
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void OnAttach();

private:
    CFont   m_font;

};

#endif // RICHVIEW_H
