//////////////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H



// Declaration of the CMyCombo class
class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo();
    virtual ~CMyCombo();
    BOOL AddItems();
    virtual void OnDestroy();
    void SetImages(int images, UINT imageID);

protected:
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CImageList m_imlImages;
};


#endif // MYCOMBO_H
