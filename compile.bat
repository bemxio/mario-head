@echo off

mkdir build

cl /Fe:"build/main.exe" /Fo:"build/main.obj" /EHsc main.cpp