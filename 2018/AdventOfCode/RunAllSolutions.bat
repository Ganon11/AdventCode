@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

FOR /l %%d IN (1, 1, 25) DO (
  SET _dir=Day%%d
  IF EXIST !_dir! (
    IF %%d EQU 14 (
      SET _command_to_run=Day%%d\Win32\Release\Day%%d.exe
    ) ELSE (
      SET _command_to_run=Day%%d\Win32\Release\Day%%d.exe -filename Day%%d\input.txt
    )
    !_command_to_run!
  )
)

ENDLOCAL