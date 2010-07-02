REM: A batch program to rebuild the Win32++ samples using Code::Blocks.
REM: The contents of the log file is erased 


REM: Set the paths
@set PATH=c:\Program Files\CodeBlocks;%PATH%
@set PATH=c:\Program Files (x86)\CodeBlocks;%PATH%

REM: Remove old files before we start
call clean
pushd "..\..\Networking"
call clean
popd

REM: Change the directory to the parent (Pushes the directory change on to the stack)
pushd ..\..
if exist "Win32++\output\Borland" rmdir /s /q "Win32++\output\Borland"
if exist "Win32++\output\VS2003" rmdir /s /q "Win32++\output\VS2003"
if exist "Win32++\output\MinGW" rmdir /s /q "Win32++\output\MinGW"


REM: Compile the code

codeblocks.exe --rebuild Browser\ProjectFiles\Browser.cbp                   >"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Dialog\ProjectFiles\Dialog.cbp                     >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild DialogBars\ProjectFiles\DialogBars.cbp             >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild DialogDemo\ProjectFiles\DialogDemo.cbp             >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild DialogTab\ProjectFiles\DialogTab.cbp               >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Dock\ProjectFiles\Dock.cbp                         >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild DockContainer\ProjectFiles\DockContainer.cbp       >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild DockTabbedMDI\ProjectFiles\DockTabbedMDI.cbp       >>"Win32++\output\CodeBlocks.log"
REM codeblocks.exe --rebuild DX\ProjectFiles\DX.cbp                         >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Explorer\ProjectFiles\Explorer.cbp                 >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild FastGDI\ProjectFiles\FastGDI.cbp                   >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild FormDemo\ProjectFiles\FormDemo.cbp                 >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Frame\ProjectFiles\Frame.cbp                       >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild MDIFrame\ProjectFiles\MDIFrame.cbp                 >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild MDIFrameDemo\ProjectFiles\MDIDemo.cbp              >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild MDIFrameSplitter\ProjectFiles\MDIFrameSplitter.cbp >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Networking\ClientDlg\Client.cbp                    >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Networking\ServerDlg\Server.cbp                    >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild NotePad\ProjectFiles\Notepad.cbp                   >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Performance\ProjectFiles\Performance.cbp           >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Picture\ProjectFiles\Picture.cbp                   >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild PropertySheet\ProjectFiles\PropertySheet.cbp       >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Scribble\ProjectFiles\Scribble.cbp                 >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Simple\ProjectFiles\Simple.cbp                     >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Splitter\ProjectFiles\Splitter.cbp                 >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild TabDemo\ProjectFiles\TabDemo.cbp                   >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Themes\ProjectFiles\Themes.cbp                     >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Threads\ProjectFiles\Threads.cbp                   >>"Win32++\output\CodeBlocks.log"
codeblocks.exe --rebuild Tray\ProjectFiles\Tray.cbp                         >>"Win32++\output\CodeBlocks.log"

REM: Copy the executables to the testing directories
mkdir "Win32++\output\VS2003"
mkdir "Win32++\output\VS2003\Debug"

ECHO "Copying VS2003 Debug files" >>"Win32++\output\CodeBlocks.log"
copy Browser\ProjectFiles\Microsoft\Debug\Browser.exe                   "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Dialog\ProjectFiles\Microsoft\Debug\Dialog.exe                     "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogBars\ProjectFiles\Microsoft\Debug\DialogBars.exe             "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogDemo\ProjectFiles\Microsoft\Debug\DialogDemo.exe             "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogTab\ProjectFiles\Microsoft\Debug\DialogTab.exe               "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Dock\ProjectFiles\Microsoft\Debug\Dock.exe                         "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DockContainer\ProjectFiles\Microsoft\Debug\DockContainer.exe       "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DockTabbedMDI\ProjectFiles\Microsoft\Debug\DockTabbedMDI.exe       "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy FormDemo\ProjectFiles\Microsoft\Debug\FormDemo.exe                 "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
REM copy DX\ProjectFiles\Microsoft\Debug\DX.exe                         "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Explorer\ProjectFiles\Microsoft\Debug\Explorer.exe                 "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy FastGDI\ProjectFiles\Microsoft\Debug\FastGDI.exe                   "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Frame\ProjectFiles\Microsoft\Debug\Frame.exe                       "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrame\ProjectFiles\Microsoft\Debug\MDIFrame.exe                 "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameDemo\ProjectFiles\Microsoft\Debug\MDIDemo.exe              "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameSplitter\ProjectFiles\Microsoft\Debug\MDIFrameSplitter.exe "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ClientDlg\Microsoft\Debug\Client.exe                    "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ServerDlg\Microsoft\Debug\Server.exe                    "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy NotePad\ProjectFiles\Microsoft\Debug\Notepad.exe                   "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Performance\ProjectFiles\Microsoft\Debug\Performance.exe           "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Picture\ProjectFiles\Microsoft\Debug\Picture.exe                   "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy PropertySheet\ProjectFiles\Microsoft\Debug\PropertySheet.exe       "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Scribble\ProjectFiles\Microsoft\Debug\Scribble.exe                 "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Simple\ProjectFiles\Microsoft\Debug\Simple.exe                     "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Splitter\ProjectFiles\Microsoft\Debug\Splitter.exe                 "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy TabDemo\ProjectFiles\Microsoft\Debug\TabDemo.exe                   "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Themes\ProjectFiles\Microsoft\Debug\Themes.exe                     "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Threads\ProjectFiles\Microsoft\Debug\Threads.exe                   "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Tray\ProjectFiles\Microsoft\Debug\Tray.exe                         "Win32++\output\VS2003\Debug"    >>"Win32++\output\CodeBlocks.log"

mkdir "Win32++\output\VS2003\Release"

ECHO "Copying VS2003 Release files" >>"Win32++\output\CodeBlocks.log"
copy Browser\ProjectFiles\Microsoft\Release\Browser.exe                   "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Dialog\ProjectFiles\Microsoft\Release\Dialog.exe                     "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogBars\ProjectFiles\Microsoft\Release\DialogBars.exe             "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogDemo\ProjectFiles\Microsoft\Release\DialogDemo.exe             "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogTab\ProjectFiles\Microsoft\Release\DialogTab.exe               "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Dock\ProjectFiles\Microsoft\Release\Dock.exe                         "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy DockContainer\ProjectFiles\Microsoft\Release\DockContainer.exe       "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy DockTabbedMDI\ProjectFiles\Microsoft\Release\DockTabbedMDI.exe       "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
REM copy DX\ProjectFiles\Microsoft\Release\DX.exe                         "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Explorer\ProjectFiles\Microsoft\Release\Explorer.exe                 "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy FastGDI\ProjectFiles\Microsoft\Release\FastGDI.exe                   "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy FormDemo\ProjectFiles\Microsoft\Release\FormDemo.exe                 "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Frame\ProjectFiles\Microsoft\Release\Frame.exe                       "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrame\ProjectFiles\Microsoft\Release\MDIFrame.exe                 "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameDemo\ProjectFiles\Microsoft\Release\MDIDemo.exe              "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameSplitter\ProjectFiles\Microsoft\Release\MDIFrameSplitter.exe "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ClientDlg\Microsoft\Release\Client.exe                    "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ServerDlg\Microsoft\Release\Server.exe                    "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy NotePad\ProjectFiles\Microsoft\Release\Notepad.exe                   "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Performance\ProjectFiles\Microsoft\Release\Performance.exe           "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Picture\ProjectFiles\Microsoft\Release\Picture.exe                   "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy PropertySheet\ProjectFiles\Microsoft\Release\PropertySheet.exe       "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Scribble\ProjectFiles\Microsoft\Release\Scribble.exe                 "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Simple\ProjectFiles\Microsoft\Release\Simple.exe                     "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Splitter\ProjectFiles\Microsoft\Release\Splitter.exe                 "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy TabDemo\ProjectFiles\Microsoft\Release\TabDemo.exe                   "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Themes\ProjectFiles\Microsoft\Release\Themes.exe                     "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Threads\ProjectFiles\Microsoft\Release\Threads.exe                   "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"
copy Tray\ProjectFiles\Microsoft\Release\Tray.exe                         "Win32++\output\VS2003\Release"    >>"Win32++\output\CodeBlocks.log"


REM: Copy the executables to the testing directories
mkdir "Win32++\output\Borland"
mkdir "Win32++\output\Borland\Debug"

ECHO "Copying Borland Debug files" >>"Win32++\output\CodeBlocks.log"
copy Browser\ProjectFiles\Borland\Debug\Browser.exe                   "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Dialog\ProjectFiles\Borland\Debug\Dialog.exe                     "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogBars\ProjectFiles\Borland\Debug\DialogBars.exe             "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogDemo\ProjectFiles\Borland\Debug\DialogDemo.exe             "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogTab\ProjectFiles\Borland\Debug\DialogTab.exe               "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Dock\ProjectFiles\Borland\Debug\Dock.exe                         "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DockContainer\ProjectFiles\Borland\Debug\DockContainer.exe       "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DockTabbedMDI\ProjectFiles\Borland\Debug\DockTabbedMDI.exe       "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
REM copy DX\ProjectFiles\Borland\Debug\DX.exe                         "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Explorer\ProjectFiles\Borland\Debug\Explorer.exe                 "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy FastGDI\ProjectFiles\Borland\Debug\FastGDI.exe                   "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy FormDemo\ProjectFiles\Borland\Debug\FormDemo.exe                 "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Frame\ProjectFiles\Borland\Debug\Frame.exe                       "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrame\ProjectFiles\Borland\Debug\MDIFrame.exe                 "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameDemo\ProjectFiles\Borland\Debug\MDIDemo.exe              "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameSplitter\ProjectFiles\Borland\Debug\MDIFrameSplitter.exe "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ClientDlg\Borland\Debug\Client.exe                    "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ServerDlg\Borland\Debug\Server.exe                    "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy NotePad\ProjectFiles\Borland\Debug\Notepad.exe                   "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Performance\ProjectFiles\Borland\Debug\Performance.exe           "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Picture\ProjectFiles\Borland\Debug\Picture.exe                   "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy PropertySheet\ProjectFiles\Borland\Debug\PropertySheet.exe       "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Scribble\ProjectFiles\Borland\Debug\Scribble.exe                 "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Simple\ProjectFiles\Borland\Debug\Simple.exe                     "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Splitter\ProjectFiles\Borland\Debug\Splitter.exe                 "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy TabDemo\ProjectFiles\Borland\Debug\TabDemo.exe                   "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Themes\ProjectFiles\Borland\Debug\Themes.exe                     "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Threads\ProjectFiles\Borland\Debug\Threads.exe                   "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Tray\ProjectFiles\Borland\Debug\Tray.exe                         "Win32++\output\Borland\Debug"    >>"Win32++\output\CodeBlocks.log"

mkdir "Win32++\output\Borland\Release"

ECHO "Copying Borland Release files" >>"Win32++\output\CodeBlocks.log"
copy Browser\ProjectFiles\Borland\Release\Browser.exe                   "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Dialog\ProjectFiles\Borland\Release\Dialog.exe                     "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogBars\ProjectFiles\Borland\Release\DialogBars.exe             "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogDemo\ProjectFiles\Borland\Release\DialogDemo.exe             "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogTab\ProjectFiles\Borland\Release\DialogTab.exe               "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Dock\ProjectFiles\Borland\Release\Dock.exe                         "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy DockContainer\ProjectFiles\Borland\Release\DockContainer.exe       "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy DockTabbedMDI\ProjectFiles\Borland\Release\DockTabbedMDI.exe       "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
REM copy DX\ProjectFiles\Borland\Release\DX.exe                         "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Explorer\ProjectFiles\Borland\Release\Explorer.exe                 "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy FastGDI\ProjectFiles\Borland\Release\FastGDI.exe                   "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy FormDemo\ProjectFiles\Borland\Release\FormDemo.exe                 "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Frame\ProjectFiles\Borland\Release\Frame.exe                       "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrame\ProjectFiles\Borland\Release\MDIFrame.exe                 "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameDemo\ProjectFiles\Borland\Release\MDIDemo.exe              "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameSplitter\ProjectFiles\Borland\Release\MDIFrameSplitter.exe "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ClientDlg\Borland\Release\Client.exe                    "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ServerDlg\Borland\Release\Server.exe                    "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy NotePad\ProjectFiles\Borland\Release\Notepad.exe                   "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Performance\ProjectFiles\Borland\Release\Performance.exe           "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Picture\ProjectFiles\Borland\Release\Picture.exe                   "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy PropertySheet\ProjectFiles\Borland\Release\PropertySheet.exe       "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Scribble\ProjectFiles\Borland\Release\Scribble.exe                 "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Simple\ProjectFiles\Borland\Release\Simple.exe                     "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Splitter\ProjectFiles\Borland\Release\Splitter.exe                 "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy TabDemo\ProjectFiles\Borland\Release\TabDemo.exe                   "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Themes\ProjectFiles\Borland\Release\Themes.exe                     "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Threads\ProjectFiles\Borland\Release\Threads.exe                   "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"
copy Tray\ProjectFiles\Borland\Release\Tray.exe                         "Win32++\output\Borland\Release"    >>"Win32++\output\CodeBlocks.log"


REM: Copy the executables to the testing directories
mkdir "Win32++\output\MinGW"
mkdir "Win32++\output\MinGW\Debug"

ECHO "Copying MinGW Debug files" >>"Win32++\output\CodeBlocks.log"
copy Browser\ProjectFiles\GNU\Debug\Browser.exe                   "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Dialog\ProjectFiles\GNU\Debug\Dialog.exe                     "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogBars\ProjectFiles\GNU\Debug\DialogBars.exe             "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogDemo\ProjectFiles\GNU\Debug\DialogDemo.exe             "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DialogTab\ProjectFiles\GNU\Debug\DialogTab.exe               "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Dock\ProjectFiles\GNU\Debug\Dock.exe                         "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DockContainer\ProjectFiles\GNU\Debug\DockContainer.exe       "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy DockTabbedMDI\ProjectFiles\GNU\Debug\DockTabbedMDI.exe       "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
REM copy DX\ProjectFiles\GNU\Debug\DX.exe                         "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Explorer\ProjectFiles\GNU\Debug\Explorer.exe                 "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy FastGDI\ProjectFiles\GNU\Debug\FastGDI.exe                   "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy FormDemo\ProjectFiles\GNU\Debug\FormDemo.exe                 "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Frame\ProjectFiles\GNU\Debug\Frame.exe                       "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrame\ProjectFiles\GNU\Debug\MDIFrame.exe                 "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameDemo\ProjectFiles\GNU\Debug\MDIDemo.exe              "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameSplitter\ProjectFiles\GNU\Debug\MDIFrameSplitter.exe "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ClientDlg\GNU\Debug\Client.exe                    "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ServerDlg\GNU\Debug\Server.exe                    "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy NotePad\ProjectFiles\GNU\Debug\Notepad.exe                   "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Performance\ProjectFiles\GNU\Debug\Performance.ex  e         "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Picture\ProjectFiles\GNU\Debug\Picture.exe                   "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy PropertySheet\ProjectFiles\GNU\Debug\PropertySheet.exe       "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Scribble\ProjectFiles\GNU\Debug\Scribble.exe                 "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Simple\ProjectFiles\GNU\Debug\Simple.exe                     "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Splitter\ProjectFiles\GNU\Debug\Splitter.exe                 "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy TabDemo\ProjectFiles\GNU\Debug\TabDemo.exe                   "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Themes\ProjectFiles\GNU\Debug\Themes.exe                     "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Threads\ProjectFiles\GNU\Debug\Threads.exe                   "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"
copy Tray\ProjectFiles\GNU\Debug\Tray.exe                         "Win32++\output\MinGW\Debug"    >>"Win32++\output\CodeBlocks.log"

mkdir "Win32++\output\MinGW\Release"

ECHO "Copying MinGW Release files" >>"Win32++\output\CodeBlocks.log"
copy Browser\ProjectFiles\GNU\Release\Browser.exe                   "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Dialog\ProjectFiles\GNU\Release\Dialog.exe                     "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogBars\ProjectFiles\GNU\Release\DialogBars.exe             "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogDemo\ProjectFiles\GNU\Release\DialogDemo.exe             "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy DialogTab\ProjectFiles\GNU\Release\DialogTab.exe               "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Dock\ProjectFiles\GNU\Release\Dock.exe                         "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy DockContainer\ProjectFiles\GNU\Release\DockContainer.exe       "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy DockTabbedMDI\ProjectFiles\GNU\Release\DockTabbedMDI.exe       "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
REM copy DX\ProjectFiles\GNU\Release\DX.exe                         "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Explorer\ProjectFiles\GNU\Release\Explorer.exe                 "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy FastGDI\ProjectFiles\GNU\Release\FastGDI.exe                   "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy FormDemo\ProjectFiles\GNU\Release\FormDemo.exe                 "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Frame\ProjectFiles\GNU\Release\Frame.exe                       "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrame\ProjectFiles\GNU\Release\MDIFrame.exe                 "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameDemo\ProjectFiles\GNU\Release\MDIDemo.exe              "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy MDIFrameSplitter\ProjectFiles\GNU\Release\MDIFrameSplitter.exe "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ClientDlg\GNU\Release\Client.exe                    "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Networking\ServerDlg\GNU\Release\Server.exe                    "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy NotePad\ProjectFiles\GNU\Release\Notepad.exe                   "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Performance\ProjectFiles\GNU\Release\Performance.exe           "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Picture\ProjectFiles\GNU\Release\Picture.exe                   "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy PropertySheet\ProjectFiles\GNU\Release\PropertySheet.exe       "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Scribble\ProjectFiles\GNU\Release\Scribble.exe                 "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Simple\ProjectFiles\GNU\Release\Simple.exe                     "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Splitter\ProjectFiles\GNU\Release\Splitter.exe                 "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy TabDemo\ProjectFiles\GNU\Release\TabDemo.exe                   "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Themes\ProjectFiles\GNU\Release\Themes.exe                     "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"
copy Threads\ProjectFiles\GNU\Release\Threads.exe                   "Win32++\output\MinGW\Release"    >>"Win32++\output\CodeBlocks.log"



REM: Pop the directory change off the stack
popd

