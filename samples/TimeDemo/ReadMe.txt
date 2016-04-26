The TimeDemo Program Features
=== ======== ======= ========

* Windowed display of document text with scrollbars as needed.  The
  text font and background color are selectable from the View menu.

* Organization of code into an architecture based on Application, Main
  Frame, Document, and View classes.

* Demonstration of the CArchive class to save the program persistent
  parameters, such as window size and placement, screen font,
  background color, toolbar visibility status, status bar visibility
  status, scrollbar parameters, and MRU list content, in a special
  file rather than in the system registry. Also, demonstration of its
  use to save and reload application documents.

* Demonstration of the CTime class usage to generate sample text
  documents.

* Use of a semaphore to limit the number of simultaneous instances of
  the running program to a given number, here only one.

* A Most Recently Used (MRU) class apart from that in the Win32++ frame
  class that does not rely on the system registry and that contains
  a ValidateMRU() method to remove elements of the list having no
  corresponding readable file.

* A skeleton of context help for the controls on the window frame,
  toolbar, and main menu.

* Features for reduced flicker when the screen is redrawn.

* A programming style that emphasizes readability, where methods are
  separated into declaration, specification, and implementation
  sections in easily identifiable blocks. All lines are limited to
  normal page size for ease in comprehending the content. Declaration
  of method names are readily identified at the leftmost column of the
  page, with specifications of a more mundane nature, such as return
  value and class name placed on an indented line above so as not to
  detract from locating the method name. Methods are generally
  presented in alphabetical order, except where special grouping is
  warranted.
