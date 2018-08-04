//////////////////////////////////////////////
// MyTabbedMDI.h

#ifndef MYTABBEDMDI_H
#define MYTABBEDMDI_H


enum MDIChildIDs
{
    ID_MDI_CLASSES = 1,
    ID_MDI_FILES = 2,
    ID_MDI_OUTPUT = 3,
    ID_MDI_TEXT = 4,
    ID_MDI_WEB = 5,
    ID_MDI_RECT = 6
};


class CMyTabbedMDI : public CTabbedMDI
{
public:
    CMyTabbedMDI();
    virtual ~CMyTabbedMDI() {}
    virtual CWnd* NewMDIChildFromID(int mdiChild);
};

#endif

