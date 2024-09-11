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
    virtual ~CDialogsTree() override;

    void FillTree(const std::vector<ResourceInfo>& allInfo, LPCWSTR fileName);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach() override;
    virtual LRESULT OnNotifyReflect(WPARAM, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDialogsTree(const CDialogsTree&) = delete;
    CDialogsTree& operator=(const CDialogsTree&) = delete;

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
    virtual ~CDockDialogsTree() override {}
    CDialogsTree* GetTree() { return &m_tree; }

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockDialogsTree(const CDockDialogsTree&) = delete;
    CDockDialogsTree& operator=(const CDockDialogsTree&) = delete;

    CDialogsTree m_tree;
};


#endif  //_DIALOGSTREE_H_
