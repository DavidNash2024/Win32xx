REM  Clean the projects folders.
call "CleanSamples.bat"
call "CleanTutorials.bat"
cmd /c call "Compile VS2022 Community.bat"

REM  Run these compile scripts concurrently.
start "" cmd /c "Compile CodeBlocks.bat"
start "" cmd /c "Compile C++ Builder.bat"
start "" cmd /c "Compile VS2026 Community.bat"

