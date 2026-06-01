/////////////////////////////
// MyCombo.h
//

#ifndef MYCOMBO_H
#define MYCOMBO_H

//////////////////////////////////////////////////////
// CMyCombo manages the ComboBoxEx control used by the
// main frame to hold the web address.
class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo() = default;
    virtual ~CMyCombo() override = default;

    const CEdit& GetCBEdit() const;


	void RestoreItems() const;
    void SaveItems();

protected:
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;

private:
    CMyCombo(const CMyCombo&) = delete;
    CMyCombo& operator=(const CMyCombo&) = delete;

    CEdit m_edit;
    std::vector<CString> m_items;
};


#endif
