/* (03-Mar-2020) [Tab/Indent: 8/8][Line/Box: 80/74]                  (MRU.cpp) *
********************************************************************************
|                                                                              |
|             Copyright (c) 2020, Robert C. Tausworthe, David Nash             |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description:  Implementation of the CMainFrm class for this 
    application using the Win32++ framework, Copyright (c) 2005-2020 David Nash, 
    under permissions granted therein.
    
    This class is used to load, display, edit, and  save the Most Recently Used 
    (MRU) list, commonly used to contain the files used by an application.
    This class has been adapted from the Win32++ framework, Copyright (c) 
    2005-2020 David Nash, under permissions granted therein.  It offers an 
    alternate to the MRU operations found in the CFrame class of Win32++ for 
    those not wishing to use the system's registry, but prefer a separate 
    application parameter file.

    The above copyright notice, as well as that of David Nash and Win32++, 
    together with the respective permissionconditions shall be included in all 
    copies or substantial portions of this material so copied, modified, merged,
    published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.
    Programming Notes:

    The programming standards roughly follow those established by the
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation
    Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "io.h"
#include "MRU.h"

/*=============================================================================*/
    CMRU::
CMRU()                                                                  /*

    Construct an empty MRU object.
*-----------------------------------------------------------------------------*/
    :   m_emptyMRUListLabel(_T("Recent Files")) 
{
    m_MRUEntries.clear(); 
    m_maxMRU = 16; 
}

/*============================================================================*/
    void CMRU::
AddEntry(LPCTSTR entryName)                                             /* 

    Add the entryName string to the Most Recently Used (MRU) menu at the
    top of the menu.  Eliminate list members if they exceed the specified
    bound on number of entries.
*-----------------------------------------------------------------------------*/
{
      // Erase possible duplicate entries from vector
    RemoveEntry(entryName);       
      // Insert the entry at the beginning of the vector
    m_MRUEntries.insert(m_MRUEntries.begin(), entryName); 
      // Delete excessive MRU entries
    if (m_MRUEntries.size() > m_maxMRU)  
        m_MRUEntries.erase(m_MRUEntries.begin() + m_maxMRU, 
            m_MRUEntries.end());        
      // display the changed MRU menu
    UpdateMenu();
}

/*============================================================================*/
    void CMRU::
AssignMenu(CMenu frameMenu, size_t maxMRU /*= 16*/)                     /* 

    Register the frame's menu and the maximum number of elements (maxMRU)
    of the MRU list to be shown. This method may be invoked at any time after
    the frame's window has been created and before any of the other operations
    of this object are invoked. The frame's OnCreate function is a good place
    for this invocation.
*-----------------------------------------------------------------------------*/
{
    m_frameMenu = frameMenu;
    assert(maxMRU > 0);    
    if (maxMRU == 0)
        maxMRU = 16;  
    m_maxMRU = maxMRU;    
    m_MRUEntries.clear(); 
}

/*============================================================================*/
    void CMRU::
EmptyList()                                                             /*

    Remove all entries from the MRU list and  display the default list
    on the MRU menu.
*-----------------------------------------------------------------------------*/
{
    m_MRUEntries.clear();  
    UpdateMenu();
}

/*============================================================================*/
    CString CMRU::
GetEntry(size_t index)                                                  /* 

    Return a MRU CString entry given its index. If the index is not within the 
    MRU list bounds, return an empty string.
*-----------------------------------------------------------------------------*/
{
    CString s = _T("");
    if (index < m_MRUEntries.size())
        s = m_MRUEntries[index];    
    return s;
}

/*============================================================================*/
    void CMRU::
RemoveEntry(LPCTSTR entryName)                                          /*

    Remove the entryName entry from the MRU list if it is in the list.
*-----------------------------------------------------------------------------*/
{
    std::vector<CString>::iterator it;
    for (it = m_MRUEntries.begin(); it != m_MRUEntries.end(); ++it) 
    {
        if ((*it) == entryName)
        {
            m_MRUEntries.erase(it); 
            break;
        }
    }
    UpdateMenu();
}

/*============================================================================*/
    void CMRU::
Serialize(CArchive &ar)                                                 /*

    Serialize or deserialize the MRU list to from the archive ar, depending
    on the sense of IsStoring(). If the archive has encountered errors at
    entry, do not execute.
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    if (ar.IsStoring())
    {
          // record the number of entries to write
        size_t entries = m_MRUEntries.size();  
        ar << entries;                         
          // save this many entries
        for (size_t i = 0; i < entries; ++i)   
        {
            ar << m_MRUEntries[i];     
        }

    }
    else    // recovering
    {
        m_MRUEntries.clear();              
        std::vector<CString> MRUEntries;   
          // load all archived MRU entries, used or not,
          // to preserve archive for other users
        size_t entries;                    
        ar >> entries; // the number of them   
        CString s;
        for (size_t i = 0; i < entries; ++i)   
        {
            ar >> s;
            if (i < m_maxMRU)  // record only those within bounds  
                MRUEntries.push_back(s);   
        }
          // successfully read in, so store them
        m_MRUEntries = MRUEntries;    
        UpdateMenu();
    }
}

/*============================================================================*/
    void CMRU::
UpdateMenu()                                                            /*

    Update the MRU menu with the current Most Recently Used (MRU) list.
    Display the MRU entries in the leftmost submenu of the CFrame's main
    menu, at the IDW_FILE_MRU_FILE1 location of that submenu, in the same
    order they occur in the current MRU CString vector.
*-----------------------------------------------------------------------------*/
{
      // find in the leftmost submenu (i.e., the one with index 0)
    CMenu fileMenu = m_frameMenu.GetSubMenu(0);
      // compute the index of the last entry in the MRU list
    int last = static_cast<int>(MIN(m_MRUEntries.size(), m_maxMRU)) -  1;   
      // if there is no leftmost submenu, or if there are no entries to
      // post, or if we cannot modify the first entry to indicate an empty
      // MRU list, we cannot proceed
    if (!fileMenu.GetHandle())
        return;

      // insert the empty MRU list label in the top slot
    fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
        IDW_FILE_MRU_FILE1, m_emptyMRUListLabel.c_str());
      // remove all the other MRU Menu entries
    for (int i = IDW_FILE_MRU_FILE2; i <= IDW_FILE_MRU_FILE1 +
        static_cast<int>(m_maxMRU); ++i)               
        fileMenu.DeleteMenu(i, MF_BYCOMMAND);
      // if the list is not empty, there's more to do
    if (last >= 0)   
    {
          // create the MRU "show" list, which contains only strings
          // of limited length, chars removed at the midpoint, as needed
        int maxLength = MAX_MENU_STRING - 10;       
        int mid = maxLength / 2;                    

        std::vector<CString> Names(m_maxMRU);              
        for (int i = 0; i <= last; i++)  
        {
            CString s = m_MRUEntries[i];     
            if (s.GetLength() > maxLength)   
            {
                  // eliminate middle if too long
                s.Delete(mid, s.GetLength() - maxLength);  
                s.Insert(mid, _T("..."));
            }
            // Prefix with its number
            CString v;
            v.Format(_T("%d "), i + 1);
            Names[i] = v + s;               
        }

          // display the MRU items: start by replacing the first item
          // in the the list with the last MRU item
        fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
            IDW_FILE_MRU_FILE1 + last, Names[last]);               
          // now insert the remaining items in reverse order, starting
          // at the next-to-iLast entry and  pushing all the others
          // down in the menu (entries thus end up in the correct order)
        for (int j = last - 1 ; j >= 0; last--, j--)              
            fileMenu.InsertMenu(IDW_FILE_MRU_FILE1 + last,        
                MF_BYCOMMAND, IDW_FILE_MRU_FILE1 + j,
                Names[j]);                                        
    }
}

/*============================================================================*/
    void CMRU::
ValidateMRU()                                                           /*

    Validate the that the MRU list entries, if there are any, correspond
    to actual file paths. Remove any that do not.
*-----------------------------------------------------------------------------*/
{
      // search the MRU list in reverse so as not to cause reshuffling
    std::vector<CString>::iterator it;
    for (int i = static_cast<int>(m_MRUEntries.size()) - 1; i >= 0; --i)   
    {
          // check whether the current entry exists, or is gone
        CString s = m_MRUEntries[i];                
        if (_taccess(s.c_str(), 4) != 0)
        {
            ::MessageBox(NULL, s, _T("Removing invalid MRU entry."),
                MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
              // convert index to the proper forward iterator for erase
            it = m_MRUEntries.begin() + i;         
            m_MRUEntries.erase(it);                
        }
    }
    UpdateMenu();
}
/*----------------------------------------------------------------------------*/

