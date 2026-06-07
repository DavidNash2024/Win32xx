BatchSourceFileEditor Sample
============================
This sample searches files within the specified folder. It does a recursive
search of all sub folders within the specified folder.

Each source file (*.cpp, *.h, *.rc) will be checked for tabs, lines ending with
spaces and non-ascii characters. The files which will be modified are listed.

When the "Fix" button is pressed, tabs are converted to 4 spaces, lines
with trailing spaces have the trailing spaces removed, and non-ascii characters
are removed.

Instructions
============
1) Make a backup copy of the files in the folder before they are  modified.

2) Press the folder button in the toolbar to choose the folder containing all
   source files to be checked. Note: subfolders will be checked as well.

3) Choose the "Check source files button" in the toolbar. This will list all
   the files that are about to be modified.
   
4) Choose the "Fix source files button" to make the modifications to the source
   files. This will replace any tabs with 4 spaces, remove trailing spaces from
   the end of each line, and remove non-ascii characters.
   
