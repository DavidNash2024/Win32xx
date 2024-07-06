@echo off
REM: A batch program to rebuild the Win32++ samples using C++ Builder 11 CE.
REM: The contents of the log file is erased 

REM: Set the paths and environment variables
call "C:\Program Files (x86)\Embarcadero\Studio\22.0\bin\rsvars.bat"

::Cleanup
call CleanSamples
call CleanTutorials

REM: Change the directory to the Samples parent
pushd ..\Samples
if exist "..\output\Embarcadero\Debug" rmdir /s /q "..\output\Embarcadero\Debug"
if exist "..\output\Embarcadero\Release" rmdir /s /q "..\output\Embarcadero\Release"
if exist "..\output\Embarcadero\Tutorials" rmdir /s /q "..\output\Embarcadero\Tutorials"

@echo on

::Compile code
msbuild Animation\ProjectFiles\Animation.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 > "..\output\Embarcadero.log"
msbuild Animation\ProjectFiles\Animation.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild Browser\ProjectFiles\Browser.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Browser\ProjectFiles\Browser.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild CommonDialogs\ProjectFiles\CommonDialogs.cbproj /t:build /p:Config=Debug /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild CommonDialogs\ProjectFiles\CommonDialogs.cbproj /t:build /p:Config=Release /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild Console\ProjectFiles\Console.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Console\ProjectFiles\Console.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild ContextHelp\ProjectFiles\ContextHelp.cbproj /t:build /p:Config=Debug /p:Platform=Win64             >>"..\output\Embarcadero.log"
msbuild ContextHelp\ProjectFiles\ContextHelp.cbproj /t:build /p:Config=Release /p:Platform=Win64           >>"..\output\Embarcadero.log"
msbuild CustomControl\ProjectFiles\CustomControl.cbproj /t:build /p:Config=Debug /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild CustomControl\ProjectFiles\CustomControl.cbproj /t:build /p:Config=Release /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild CustomPrintDlg\ProjectFiles\CustomPrintDlg.cbproj /t:build /p:Config=Debug /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild CustomPrintDlg\ProjectFiles\CustomPrintDlg.cbproj /t:build /p:Config=Release /p:Platform=Win64     >>"..\output\Embarcadero.log"
msbuild DateTime\ProjectFiles\DateTime.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild DateTime\ProjectFiles\DateTime.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild DDXDemo\ProjectFiles\DDXDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild DDXDemo\ProjectFiles\DDXDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild Dialog\ProjectFiles\Dialog.cbproj /t:build /p:Config=Debug /p:Platform=Win64                       >>"..\output\Embarcadero.log"
msbuild Dialog\ProjectFiles\Dialog.cbproj /t:build /p:Config=Release /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild DialogBars\ProjectFiles\DialogBars.cbproj /t:build /p:Config=Debug /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild DialogBars\ProjectFiles\DialogBars.cbproj /t:build /p:Config=Release /p:Platform=Win64             >>"..\output\Embarcadero.log"
msbuild DialogDemo\ProjectFiles\DialogDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild DialogDemo\ProjectFiles\DialogDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64             >>"..\output\Embarcadero.log"
msbuild DialogResizing\ProjectFiles\DialogResizing.cbproj /t:build /p:Config=Debug /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild DialogResizing\ProjectFiles\DialogResizing.cbproj /t:build /p:Config=Release /p:Platform=Win64     >>"..\output\Embarcadero.log"
msbuild DialogTab\ProjectFiles\DialogTab.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild DialogTab\ProjectFiles\DialogTab.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild DialogTemplate\ProjectFiles\DialogTemplate.cbproj /t:build /p:Config=Debug /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild DialogTemplate\ProjectFiles\DialogTemplate.cbproj /t:build /p:Config=Release /p:Platform=Win64     >>"..\output\Embarcadero.log"
msbuild Dock\ProjectFiles\Dock.cbproj /t:build /p:Config=Debug /p:Platform=Win64                           >>"..\output\Embarcadero.log"
msbuild Dock\ProjectFiles\Dock.cbproj /t:build /p:Config=Release /p:Platform=Win64                         >>"..\output\Embarcadero.log"
msbuild DockContainer\ProjectFiles\DockContainer.cbproj /t:build /p:Config=Debug /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild DockContainer\ProjectFiles\DockContainer.cbproj /t:build /p:Config=Release /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild DockTabbedMDI\ProjectFiles\DockTabbedMDI.cbproj /t:build /p:Config=Debug /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild DockTabbedMDI\ProjectFiles\DockTabbedMDI.cbproj /t:build /p:Config=Release /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild DoubleBuffer\ProjectFiles\DoubleBuffer.cbproj /t:build /p:Config=Debug /p:Platform=Win64           >>"..\output\Embarcadero.log"
msbuild DoubleBuffer\ProjectFiles\DoubleBuffer.cbproj /t:build /p:Config=Release /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild Explorer\ProjectFiles\Explorer.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild Explorer\ProjectFiles\Explorer.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild FastGDI\ProjectFiles\FastGDI.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild FastGDI\ProjectFiles\FastGDI.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild FilePrintPreview\ProjectFiles\FilePrintPreview.cbproj /t:build /p:Config=Debug /p:Platform=Win64   >>"..\output\Embarcadero.log"
msbuild FilePrintPreview\ProjectFiles\FilePrintPreview.cbproj /t:build /p:Config=Release /p:Platform=Win64 >>"..\output\Embarcadero.log"
msbuild FormDocView\ProjectFiles\FormDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64                >>"..\output\Embarcadero.log"
msbuild FormDocView\ProjectFiles\FormDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64              >>"..\output\Embarcadero.log"
msbuild Frame\ProjectFiles\Frame.cbproj /t:build /p:Config=Debug /p:Platform=Win64                         >>"..\output\Embarcadero.log"
msbuild Frame\ProjectFiles\Frame.cbproj /t:build /p:Config=Release /p:Platform=Win64                       >>"..\output\Embarcadero.log"
msbuild FrameEx\ProjectFiles\FrameEx.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild FrameEx\ProjectFiles\FrameEx.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild GDIPlus\ProjectFiles\GDIPlus.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild GDIPlus\ProjectFiles\GDIPlus.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild INIFrame\ProjectFiles\INIFrame.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild INIFrame\ProjectFiles\INIFrame.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild ListView\ProjectFiles\ListView.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild ListView\ProjectFiles\ListView.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild MDIDockFrame\ProjectFiles\MDIDockFrame.cbproj /t:build /p:Config=Debug /p:Platform=Win64           >>"..\output\Embarcadero.log"
msbuild MDIDockFrame\ProjectFiles\MDIDockFrame.cbproj /t:build /p:Config=Release /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild MDIFrame\ProjectFiles\MDIFrame.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild MDIFrame\ProjectFiles\MDIFrame.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild MDIFrameDemo\ProjectFiles\MDIDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64                >>"..\output\Embarcadero.log"
msbuild MDIFrameDemo\ProjectFiles\MDIDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64              >>"..\output\Embarcadero.log"
msbuild MDIFrameSplitter\ProjectFiles\MDIFrameSplitter.cbproj /t:build /p:Config=Debug /p:Platform=Win64   >>"..\output\Embarcadero.log"
msbuild MDIFrameSplitter\ProjectFiles\MDIFrameSplitter.cbproj /t:build /p:Config=Release /p:Platform=Win64 >>"..\output\Embarcadero.log"
msbuild MetaFile\ProjectFiles\MetaFile.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild MetaFile\ProjectFiles\MetaFile.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild MovieShow\ProjectFiles\MovieShow.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild MovieShow\ProjectFiles\MovieShow.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild NetClient\ProjectFiles\NetClient.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild NetClient\ProjectFiles\NetClient.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild NetClientAsync\ProjectFiles\NetClientAsync.cbproj /t:build /p:Config=Debug /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild NetClientAsync\ProjectFiles\NetClientAsync.cbproj /t:build /p:Config=Release /p:Platform=Win64     >>"..\output\Embarcadero.log"
msbuild NetServer\ProjectFiles\NetServer.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild NetServer\ProjectFiles\NetServer.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild NetServerAsync\ProjectFiles\NetServerAsync.cbproj /t:build /p:Config=Debug /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild NetServerAsync\ProjectFiles\NetServerAsync.cbproj /t:build /p:Config=Release /p:Platform=Win64     >>"..\output\Embarcadero.log"
msbuild Notepad\ProjectFiles\Notepad.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Notepad\ProjectFiles\Notepad.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild Performance\ProjectFiles\Performance.cbproj /t:build /p:Config=Debug /p:Platform=Win64             >>"..\output\Embarcadero.log"
msbuild Performance\ProjectFiles\Performance.cbproj /t:build /p:Config=Release /p:Platform=Win64           >>"..\output\Embarcadero.log"
msbuild Picture\ProjectFiles\Picture.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Picture\ProjectFiles\Picture.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild PropertySheet\ProjectFiles\PropertySheet.cbproj /t:build /p:Config=Debug /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild PropertySheet\ProjectFiles\PropertySheet.cbproj /t:build /p:Config=Release /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild ReBarDemo\ProjectFiles\ReBarDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild ReBarDemo\ProjectFiles\ReBarDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild RoundWindow\ProjectFiles\Round.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild RoundWindow\ProjectFiles\Round.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild ScintillaDemo\ProjectFiles\ScintillaDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64         >>"..\output\Embarcadero.log"
msbuild ScintillaDemo\ProjectFiles\ScintillaDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild Scribble\ProjectFiles\Scribble.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild Scribble\ProjectFiles\Scribble.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild Simple\ProjectFiles\Simple.cbproj /t:build /p:Config=Debug /p:Platform=Win64                       >>"..\output\Embarcadero.log"
msbuild Simple\ProjectFiles\Simple.cbproj /t:build /p:Config=Release /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Splitter\ProjectFiles\Splitter.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild Splitter\ProjectFiles\Splitter.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild StatusBar\ProjectFiles\StatusBar.cbproj /t:build /p:Config=Debug /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild StatusBar\ProjectFiles\StatusBar.cbproj /t:build /p:Config=Release /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild TabDemo\ProjectFiles\TabDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild TabDemo\ProjectFiles\TabDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild TaskDialog\ProjectFiles\TaskDialog.cbproj /t:build /p:Config=Debug /p:Platform=Win64               >>"..\output\Embarcadero.log"
msbuild TaskDialog\ProjectFiles\TaskDialog.cbproj /t:build /p:Config=Release /p:Platform=Win64             >>"..\output\Embarcadero.log"
msbuild TextFileReader\ProjectFiles\TextFileReader.cbproj /t:build /p:Config=Debug /p:Platform=Win64       >>"..\output\Embarcadero.log"
msbuild TextFileReader\ProjectFiles\TextFileReader.cbproj /t:build /p:Config=Release /p:Platform=Win64     >>"..\output\Embarcadero.log"
msbuild Themes\ProjectFiles\Themes.cbproj /t:build /p:Config=Debug /p:Platform=Win64                       >>"..\output\Embarcadero.log"
msbuild Themes\ProjectFiles\Themes.cbproj /t:build /p:Config=Release /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Threads\ProjectFiles\Threads.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild Threads\ProjectFiles\Threads.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild TimeDemo\ProjectFiles\TimeDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild TimeDemo\ProjectFiles\TimeDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild ToolBarDemo\ProjectFiles\ToolBarDemo.cbproj /t:build /p:Config=Debug /p:Platform=Win64             >>"..\output\Embarcadero.log"
msbuild ToolBarDemo\ProjectFiles\ToolBarDemo.cbproj /t:build /p:Config=Release /p:Platform=Win64           >>"..\output\Embarcadero.log"
msbuild ToolTips\ProjectFiles\ToolTips.cbproj /t:build /p:Config=Debug /p:Platform=Win64                   >>"..\output\Embarcadero.log"
msbuild ToolTips\ProjectFiles\ToolTips.cbproj /t:build /p:Config=Release /p:Platform=Win64                 >>"..\output\Embarcadero.log"
msbuild Tray\ProjectFiles\Tray.cbproj /t:build /p:Config=Debug /p:Platform=Win64                           >>"..\output\Embarcadero.log"
msbuild Tray\ProjectFiles\Tray.cbproj /t:build /p:Config=Release /p:Platform=Win64                         >>"..\output\Embarcadero.log"
msbuild WinPlot\ProjectFiles\WinPlot.cbproj /t:build /p:Config=Debug /p:Platform=Win64                     >>"..\output\Embarcadero.log"
msbuild WinPlot\ProjectFiles\WinPlot.cbproj /t:build /p:Config=Release /p:Platform=Win64                   >>"..\output\Embarcadero.log"

mkdir "..\output\Embarcadero"
mkdir "..\output\Embarcadero\Debug"

ECHO "Copying Debug Samples" >>"..\output\Embarcadero.log"
copy Animation\ProjectFiles\Embarcadero\Win64\Debug\Animation.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Browser\ProjectFiles\Embarcadero\Win64\Debug\Browser.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy CommonDialogs\ProjectFiles\Embarcadero\Win64\Debug\CommonDialogs.exe       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Console\ProjectFiles\Embarcadero\Win64\Debug\Console.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ContextHelp\ProjectFiles\Embarcadero\Win64\Debug\ContextHelp.exe           "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ContextHelp\Setup\ContextHelp.chm                                          "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy CustomControl\ProjectFiles\Embarcadero\Win64\Debug\CustomControl.exe       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy CustomPrintDlg\ProjectFiles\Embarcadero\Win64\Debug\CustomPrintDlg.exe     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DateTime\ProjectFiles\Embarcadero\Win64\Debug\DateTime.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DDXDemo\ProjectFiles\Embarcadero\Win64\Debug\DDXDemo.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Dialog\ProjectFiles\Embarcadero\Win64\Debug\Dialog.exe                     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DialogBars\ProjectFiles\Embarcadero\Win64\Debug\DialogBars.exe             "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DialogDemo\ProjectFiles\Embarcadero\Win64\Debug\DialogDemo.exe             "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DialogResizing\ProjectFiles\Embarcadero\Win64\Debug\DialogResizing.exe     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DialogTab\ProjectFiles\Embarcadero\Win64\Debug\DialogTab.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DialogTemplate\ProjectFiles\Embarcadero\Win64\Debug\DialogTemplate.exe     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Dock\ProjectFiles\Embarcadero\Win64\Debug\Dock.exe                         "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DockContainer\ProjectFiles\Embarcadero\Win64\Debug\DockContainer.exe       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DockTabbedMDI\ProjectFiles\Embarcadero\Win64\Debug\DockTabbedMDI.exe       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy DoubleBuffer\ProjectFiles\Embarcadero\Win64\Debug\DoubleBuffer.exe         "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Explorer\ProjectFiles\Embarcadero\Win64\Debug\Explorer.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy FastGDI\ProjectFiles\Embarcadero\Win64\Debug\FastGDI.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy FilePrintPreview\ProjectFiles\Embarcadero\Win64\Debug\FilePrintPreview.exe "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy FormDocView\ProjectFiles\Embarcadero\Win64\Debug\FormDemo.exe              "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Frame\ProjectFiles\Embarcadero\Win64\Debug\Frame.exe                       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy FrameEx\ProjectFiles\Embarcadero\Win64\Debug\FrameEx.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy GDIPlus\ProjectFiles\Embarcadero\Win64\Debug\GDIPlus.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy INIFrame\ProjectFiles\Embarcadero\Win64\Debug\INIFrame.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ListView\ProjectFiles\Embarcadero\Win64\Debug\ListView.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MDIDockFrame\ProjectFiles\Embarcadero\Win64\Debug\MDIDockFrame.exe         "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MDIFrame\ProjectFiles\Embarcadero\Win64\Debug\MDIFrame.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MDIFrameDemo\ProjectFiles\Embarcadero\Win64\Debug\MDIDemo.exe              "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MDIFrameSplitter\ProjectFiles\Embarcadero\Win64\Debug\MDIFrameSplitter.exe "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MetaFile\ProjectFiles\Embarcadero\Win64\Debug\MetaFile.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MovieShow\ProjectFiles\Embarcadero\Win64\Debug\MovieShow.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy MovieShow\install\files\MediaInfo.dll                                      "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy NetClient\ProjectFiles\Embarcadero\Win64\Debug\NetClient.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy NetClientAsync\ProjectFiles\Embarcadero\Win64\Debug\NetClientAsync.exe     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy NetServer\ProjectFiles\Embarcadero\Win64\Debug\NetServer.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy NetServerAsync\ProjectFiles\Embarcadero\Win64\Debug\NetServerAsync.exe     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Notepad\ProjectFiles\Embarcadero\Win64\Debug\Notepad.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Performance\ProjectFiles\Embarcadero\Win64\Debug\Performance.exe           "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Picture\ProjectFiles\Embarcadero\Win64\Debug\Picture.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ReBarDemo\ProjectFiles\Embarcadero\Win64\Debug\ReBarDemo.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy RoundWindow\ProjectFiles\Embarcadero\Win64\Debug\Round.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ScintillaDemo\ProjectFiles\Embarcadero\Win64\Debug\ScintillaDemo.exe       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ScintillaDemo\DLLs\x64\Scintilla.dll                                       "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Scribble\ProjectFiles\Embarcadero\Win64\Debug\Scribble.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Simple\ProjectFiles\Embarcadero\Win64\Debug\Simple.exe                     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Splitter\ProjectFiles\Embarcadero\Win64\Debug\Splitter.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy StatusBar\ProjectFiles\Embarcadero\Win64\Debug\StatusBar.exe               "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy TabDemo\ProjectFiles\Embarcadero\Win64\Debug\TabDemo.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy TaskDialog\ProjectFiles\Embarcadero\Win64\Debug\TaskDialog.exe             "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy TextFileReader\ProjectFiles\Embarcadero\Win64\Debug\TextFileReader.exe     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Themes\ProjectFiles\Embarcadero\Win64\Debug\Themes.exe                     "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Threads\ProjectFiles\Embarcadero\Win64\Debug\Threads.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy TimeDemo\ProjectFiles\Embarcadero\Win64\Debug\TimeDemo.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ToolBarDemo\ProjectFiles\Embarcadero\Win64\Debug\ToolBarDemo.exe           "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy ToolTips\ProjectFiles\Embarcadero\Win64\Debug\ToolTips.exe                 "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy Tray\ProjectFiles\Embarcadero\Win64\Debug\Tray.exe                         "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"
copy WinPlot\ProjectFiles\Embarcadero\Win64\Debug\WinPlot.exe                   "..\output\Embarcadero\Debug"    >>"..\output\Embarcadero.log"

mkdir "..\output\Embarcadero\Release"

ECHO "Copying Release Samples" >>"..\output\Embarcadero.log"
copy Animation\ProjectFiles\Embarcadero\Win64\Release\Animation.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Browser\ProjectFiles\Embarcadero\Win64\Release\Browser.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy CommonDialogs\ProjectFiles\Embarcadero\Win64\Release\CommonDialogs.exe       "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Console\ProjectFiles\Embarcadero\Win64\Release\Console.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ContextHelp\ProjectFiles\Embarcadero\Win64\Release\ContextHelp.exe           "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy CustomControl\ProjectFiles\Embarcadero\Win64\Release\CustomControl.exe       "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ContextHelp\Setup\ContextHelp.chm                                            "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy CustomPrintDlg\ProjectFiles\Embarcadero\Win64\Release\CustomPrintDlg.exe     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DateTime\ProjectFiles\Embarcadero\Win64\Release\DateTime.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DDXDemo\ProjectFiles\Embarcadero\Win64\Release\DDXDemo.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Dialog\ProjectFiles\Embarcadero\Win64\Release\Dialog.exe                     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DialogBars\ProjectFiles\Embarcadero\Win64\Release\DialogBars.exe             "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DialogDemo\ProjectFiles\Embarcadero\Win64\Release\DialogDemo.exe             "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DialogResizing\ProjectFiles\Embarcadero\Win64\Release\DialogResizing.exe     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DialogTab\ProjectFiles\Embarcadero\Win64\Release\DialogTab.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DialogTemplate\ProjectFiles\Embarcadero\Win64\Release\DialogTemplate.exe     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Dock\ProjectFiles\Embarcadero\Win64\Release\Dock.exe                         "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DockContainer\ProjectFiles\Embarcadero\Win64\Release\DockContainer.exe       "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DockTabbedMDI\ProjectFiles\Embarcadero\Win64\Release\DockTabbedMDI.exe       "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy DoubleBuffer\ProjectFiles\Embarcadero\Win64\Release\DoubleBuffer.exe         "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Explorer\ProjectFiles\Embarcadero\Win64\Release\Explorer.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy FastGDI\ProjectFiles\Embarcadero\Win64\Release\FastGDI.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy FilePrintPreview\ProjectFiles\Embarcadero\Win64\Release\FilePrintPreview.exe "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy FormDocView\ProjectFiles\Embarcadero\Win64\Release\FormDemo.exe              "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Frame\ProjectFiles\Embarcadero\Win64\Release\Frame.exe                       "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy FrameEx\ProjectFiles\Embarcadero\Win64\Release\FrameEx.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy GDIPlus\ProjectFiles\Embarcadero\Win64\Release\GDIPlus.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy INIFrame\ProjectFiles\Embarcadero\Win64\Release\INIFrame.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ListView\ProjectFiles\Embarcadero\Win64\Release\ListView.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MDIDockFrame\ProjectFiles\Embarcadero\Win64\Release\MDIDockFrame.exe         "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MDIFrame\ProjectFiles\Embarcadero\Win64\Release\MDIFrame.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MDIFrameDemo\ProjectFiles\Embarcadero\Win64\Release\MDIDemo.exe              "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MDIFrameSplitter\ProjectFiles\Embarcadero\Win64\Release\MDIFrameSplitter.exe "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MetaFile\ProjectFiles\Embarcadero\Win64\Release\MetaFile.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MovieShow\ProjectFiles\Embarcadero\Win64\Release\MovieShow.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy MovieShow\install\files\MediaInfo.dll                                        "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy NetClient\ProjectFiles\Embarcadero\Win64\Release\NetClient.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy NetClientAsync\ProjectFiles\Embarcadero\Win64\Release\NetClientAsync.exe     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy NetServer\ProjectFiles\Embarcadero\Win64\Release\NetServer.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy NetServerAsync\ProjectFiles\Embarcadero\Win64\Release\NetServerAsync.exe     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Notepad\ProjectFiles\Embarcadero\Win64\Release\Notepad.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Performance\ProjectFiles\Embarcadero\Win64\Release\Performance.exe           "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Picture\ProjectFiles\Embarcadero\Win64\Release\Picture.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ReBarDemo\ProjectFiles\Embarcadero\Win64\Release\ReBarDemo.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy RoundWindow\ProjectFiles\Embarcadero\Win64\Release\Round.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ScintillaDemo\ProjectFiles\Embarcadero\Win64\Release\ScintillaDemo.exe       "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ScintillaDemo\DLLs\x64\Scintilla.dll                                         "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Scribble\ProjectFiles\Embarcadero\Win64\Release\Scribble.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Simple\ProjectFiles\Embarcadero\Win64\Release\Simple.exe                     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Splitter\ProjectFiles\Embarcadero\Win64\Release\Splitter.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy StatusBar\ProjectFiles\Embarcadero\Win64\Release\StatusBar.exe               "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy TabDemo\ProjectFiles\Embarcadero\Win64\Release\TabDemo.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy TaskDialog\ProjectFiles\Embarcadero\Win64\Release\TaskDialog.exe             "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy TextFileReader\ProjectFiles\Embarcadero\Win64\Release\TextFileReader.exe     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Themes\ProjectFiles\Embarcadero\Win64\Release\Themes.exe                     "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Threads\ProjectFiles\Embarcadero\Win64\Release\Threads.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy TimeDemo\ProjectFiles\Embarcadero\Win64\Release\TimeDemo.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ToolBarDemo\ProjectFiles\Embarcadero\Win64\Release\ToolBarDemo.exe           "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy ToolTips\ProjectFiles\Embarcadero\Win64\Release\ToolTips.exe                 "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy Tray\ProjectFiles\Embarcadero\Win64\Release\Tray.exe                         "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"
copy WinPlot\ProjectFiles\Embarcadero\Win64\Release\WinPlot.exe                   "..\output\Embarcadero\Release"    >>"..\output\Embarcadero.log"

REM: Pop the directory change off the stack
popd


