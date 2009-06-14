
CPropertyPage and CPropertySheet

A property sheet will have one or more property pages. These pages are much 
like dialogs which are presented within a tabbed dialog or within a wizard. 
The data on a property page can be validated before the next page is presented.
Property sheets have three modes of use: Modal, Modeless, and Wizard.

In this demo CMyProptertySheet inherrits from CMyPropertySheet. It displays
two property pages, namely CButtonPage and CComboPage. The demo shows how a
Modal, Modeless or Wizard property sheet can be displayed.
 