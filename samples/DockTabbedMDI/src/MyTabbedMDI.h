/////////////////////////////
// MyTabbedMDI.h
//

#ifndef MYTABBEDMDI_H
#define MYTABBEDMDI_H

// The docker identifiers (dock IDs)
const int ID_MDI_CLASSES = 1;
const int ID_MDI_FILES = 2;
const int ID_MDI_OUTPUT = 3;
const int ID_MDI_TEXT = 4;
const int ID_MDI_WEB = 5;
const int ID_MDI_RECT = 6;


////////////////////////////////////////
// Declaration of the CMyTabbedMDI class
//
class CMyTabbedMDI : public CTabbedMDI
{
public:
    CMyTabbedMDI();
    virtual ~CMyTabbedMDI() override {}

protected:
    // Virtual functions that override base class functions.
    virtual WndPtr NewMDIChildFromID(int mdiChild) override;
    virtual LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyTabbedMDI(const CMyTabbedMDI&) = delete;
    CMyTabbedMDI& operator=(const CMyTabbedMDI&) = delete;
};

#endif
