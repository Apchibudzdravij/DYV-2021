call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat"
cd DYV-2021
ml /c /coff /Zi "project.asm"
link /OPT:NOREF /DEBUG "project.obj" "../Debug/StaticLib2.lib" /SUBSYSTEM:CONSOLE /NODEFAULTLIB:libcurt.lib
call project.exe