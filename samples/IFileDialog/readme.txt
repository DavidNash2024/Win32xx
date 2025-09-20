IFileDialog sample
==================
This sample illustrates the use of the IFileOpenDialog, IFileSaveDialog,
and associated interfaces) to create a custom file open/save dialog.

The IFileDialog and associated interfaces is the modern alternative to the
classic GetOpenFileName and GetSaveFileName common dialogs. The IFileDialog
interface has more flexibility than the classic common dialogs, but the
appearance of the basic dialogs provided by both methods is identical. 

The dialogs provided by IFileDialog are more configurable, and allow additional
controls can be added to the dialog. This extra functionality can make the
dialogs created by IFileDialog interface more complex to implement than the
classic common dialogs.

The IFileDialog interface can be used to display and choose folders. This makes
the IFileDialog interface a useful and modern alternative to SHBrowseForFolder
and CFolderDialog.

This sample is based on the CommonFileDialog and the CommFileDialogModes
samples that ships with the Windows 7 SDK. The original sample can be
downloaded from:
https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/CommonFileDialogModes
https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/winui/shell/appplatform/commonfiledialog

Using the IFileDialog sample
----------------------------
This program displays a taskdialog that provides the following options:
* Choose File
* Choose Folder
* Choose Multiple Files
* Add Custom Controls
* Set Properties for File Type
* Write Properties Using Handlers
* Write Properties without Using Handlers

Each of these options use individual code snippets. They demonstrate the
different ways in which to customize the appearance and behavior of
IFileOpenDialog and IFileSaveDialog.

The first two options demonstrate a comparatively simple use of IFileOpenDialog
for choosing files or folders. The methods demonstrated there would be
sufficient for most purposes.

The remaining options use the IFileDialogEvents and IFileDialogControlEvents
interfaces. These interfaces exposes methods that allow the application to
be notified of events that are related to controls that the application has
added to the file dialogs.

To use the "Write Properties Using Handlers" option, place a jpg image called
Flower.jpg in the "%PUBLIC%\Pictures\Sample Pictures" folder. This folder is
usually "C:\Users\Public\Pictures\Sample Pictures". This option then displays
a dialog which allows the user to choose new properties for Flower.jpg and 
select a destination folder write the file to.

This code uses the ComPtr smart pointer from the Windows SDK version 8.1 to 
manage the COM interface pointers. The Windows SDK version 8.1 ships as part
of VS2019, VS2022 and the later versions of MinGW-64. 
