/////////////////////////////
// MyCombo.h
//

#ifndef MYCOMBO_H
#define MYCOMBO_H


//////////////////////////////////////////////////////////////
// CMyCombo manages the ComboBoxEx control used by the toolbar
// in CContainClasses.
class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo();
    virtual ~CMyCombo();
    BOOL AddItems();
    void SetImages(int images, UINT imageID);

protected:
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CImageList m_images;
};


#endif // MYCOMBO_H
