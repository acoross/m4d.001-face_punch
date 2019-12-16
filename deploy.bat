@echo off
if not exist "deploy" mkdir deploy
del /f /q deploy\*.*
robocopy /njs /np /njh /nfl /nc /ndl /ns x64/Debug deploy
copy external\sfml\lib\*.dll deploy\
copy Resources\*.* deploy\