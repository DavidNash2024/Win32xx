//////////////////////////////////////////////////////////
// DialogsTree.h - Declaration of CDialogsTree,
//                 and CDockDialogsTree classes
//

#ifndef _DIALOGSTREE_H_
#define _DIALOGSTREE_H_

#include "ResourceFinder.h"


///////////////////////////////////////////////
// CDialogsTree manages a tree view control.
// This is the view window for CDockDialogsTree,
// and is used to display a list of dialogs.
class CDialogsTree : public CTreeView
{
public:
    CDialogsTree();
    virtual ~CDialogsTree();

    void FillTree(const std::vector<ResourceInfo>& allInfo, LPCTSTR fileName);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT OnNotifyReflect(WPARAM, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);

private:
    CDialogsTree(const CDialogsTree&);                // Disable copy construction
    CDialogsTree& operator = (const CDialogsTree&);   // Disable assignment operator

    // Private member functions
    BOOL OnSelChanged();

    // Private member variables
    CImageList m_normalImages;
};

/////////////////////////////////////////
// CDockDialogsTree manages a docker. It uses
// CDialogsTree as its view.
class CDockDialogsTree : public CDocker
{
public:
    CDockDialogsTree();
    virtual ~CDockDialogsTree() {}
    CDialogsTree* GetTree() { return &m_tree; }

private:
    CDockDialogsTree(const CDockDialogsTree&);                // Disable copy construction
    CDockDialogsTree& operator = (const CDockDialogsTree&);   // Disable assignment operator

    CDialogsTree m_tree;
};


#endif  //_DIALOGSTREE_H_
