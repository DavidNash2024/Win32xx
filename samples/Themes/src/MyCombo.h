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

protected:
    // Virtual functions that override base class functions
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CMyCombo(const CMyCombo&);                // Disable copy construction
    CMyCombo& operator = (const CMyCombo&);   // Disable assignment operator

    void SetImage(int image, UINT imageID);

    // Member variables
    CImageList m_images;
};


#endif // MYCOMBO_H
