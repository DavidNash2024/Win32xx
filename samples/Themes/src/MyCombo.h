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
    CMyCombo() = default;
    virtual ~CMyCombo() override = default;
    BOOL AddItems();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyCombo(const CMyCombo&) = delete;
    CMyCombo& operator=(const CMyCombo&) = delete;
    void SetImages(UINT bitmapID, int imageCount);

    // Member variables
    CImageList m_images;
};


#endif // MYCOMBO_H
