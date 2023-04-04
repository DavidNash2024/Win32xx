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
    void SetImages(int image, UINT imageID);

private:
    CMyCombo(const CMyCombo&);                // Disable copy construction
    CMyCombo& operator = (const CMyCombo&);   // Disable assignment operator

    // Member variables
    CImageList m_images;
};


#endif // MYCOMBO_H
