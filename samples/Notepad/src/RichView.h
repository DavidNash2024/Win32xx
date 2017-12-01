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
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CFont   m_Font;

};

#endif // RICHVIEW_H
