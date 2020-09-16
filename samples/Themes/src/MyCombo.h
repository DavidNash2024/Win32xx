/////////////////////////////
// MyCombo.h
//

#ifndef MYCOMBO_H
#define MYCOMBO_H


/////////////////////////////////////////////////////////
// CMyCombo manages the ComboBoxEx control, It is used by
// the toolbar.
class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo();
    virtual ~CMyCombo();
    BOOL AddItems();
    void SetImage(int image, UINT imageID);

protected:
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CImageList m_images;
};


#endif // MYCOMBO_H
