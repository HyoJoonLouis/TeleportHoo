@echo off
echo Cleaning

powershell.exe -Command "Get-ChildItem -Recurse -Include 'Build', 'Intermediate', 'Binaries', '*.sln', '.vs' | Remove-Item -Force -Recurse"