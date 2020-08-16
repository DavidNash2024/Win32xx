/////////////////////////////
// MyCombo.h
//

#ifndef MYCOMBO_H
#define MYCOMBO_H

/////////////////////////////////////
// Declaration of the CMyCombo class.
//
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
