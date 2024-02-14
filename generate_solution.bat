@echo off
cd /d "%~dp0
start /wait "" "%~dp0\Libraries\vendors\premake\premake5.exe" vs2022
