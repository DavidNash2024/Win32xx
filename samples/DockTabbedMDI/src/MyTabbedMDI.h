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
    virtual ~CMyTabbedMDI() {}

protected:
    // Virtual functions that override base class functions
    virtual CWnd* NewMDIChildFromID(int mdiChild);

private:
    CMyTabbedMDI(const CMyTabbedMDI&);                // Disable copy construction
    CMyTabbedMDI& operator = (const CMyTabbedMDI&);   // Disable assignment operator
};

#endif

