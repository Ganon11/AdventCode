@ECHO OFF
SETLOCAL
IF %1 EQU 14 (
  SET command=Day%1\Win32\Release\Day%1.exe
) ELSE (
  SET command=Day%1\Win32\Release\Day%1.exe -filename Day%1\Input.txt
)
%command%