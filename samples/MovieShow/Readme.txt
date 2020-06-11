MovieShow Example
=================
This program manages a video library.

The code assumes the video files have an m4v extension and already have
the relevant meta data. The meta data includes the movie name, actors,
genre, plot description, cover image, etc.

The frame uses a toolbar without a menu. The frame's view has resizable
treeview, listview and dialog windows. Popup menus are initiated with
a right mouse button click on the listview and the boxsets in the
treeview.

The program uses a std::list of the MediaInfo struct for its video library.
It uses this as a simple database to extract movies by genre, release date,
and box sets.

The Add Folder button updates the media library. New files are added,
deleted files are removed, and modified files are updated.

A splash screen is used to announce time consuming tasks, such as loading
the library, updating the library, and saving the library.

The code uses MediaInfo.dll. This is 3ed party software available from
MediaArea at:
https://mediaarea.net/en/MediaInfo/Download/Windows

The install folder contains a NSIS (Nullsoft Scriptable Install System)
project file. NSIS can be used to build a Windows setup file for the
MovieShow application. This file will register MovieShow in the Windows
Start Menu and provide an uninstall file. 

A compiled setup file (SetupMovieShow.exe) is provided on the install folder
which installs a 64 bit version of MovieShow. 

NSIS is 3rd party software available from:
https://sourceforge.net/projects/nsis/


Features demonstrated in this example
=====================================
* Only one instance of the program is allowed to run.
* Implementation of a splash screen with a progress bar.
* Use of CFrame with a toolbar, but without a menu
* The view window of the frame has a resizable dialog, a TreeView and 
   a ListView, separated by splitter bars.
* Forward and reverse sorting of list view items by clicking the header control.
* Use if CMenu to create popup menus.
* Use of CFileFind to iterate through files in a directory
* Use of a worker thread to run a background task
* Using MediaInfo.dll to extract meta data from video files
* Use of CryptStringToBinary to convert an image to a sequence of bytes.
* Use of IStream to load an image.
* Use of GDIPlus to draw an image from an IStream.
* Use of ShellExecute to play a video file.
* Use of CArchive to load and store video files information.
* Ability to retrieve video files based on title, release date, genre, 
   box set, and file name. 
* How to use Nullsoft Scriptable Install System to install the
   program as a windows application.   
 

