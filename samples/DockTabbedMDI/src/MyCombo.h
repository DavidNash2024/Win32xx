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

protected:
    // Virtual functions that override base class functions
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    void SetImages(int images, UINT imageID);

    // Member variables
    CImageList m_images;
};


#endif // MYCOMBO_H
