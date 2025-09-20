/////////////////////////////
// Classes.h
//

#ifndef CLASSES_H
#define CLASSES_H


//////////////////////////////////////////////////
// CViewClasses manages a tree view control.
// It is used as a view window by the tab control.
class CViewClasses : public CTreeView
{
public:
    CViewClasses() = default;
    virtual ~CViewClasses() override;
    void SetDPIImages();

protected:
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewClasses(const CViewClasses&) = delete;
    CViewClasses& operator=(const CViewClasses&) = delete;

    // Command handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif  //CLASSES_H
