#!/bin/bash

cd ../../

cp -R assets $1/
cp -R resources $1/

#if [ -e $1.kdev4 ]
#then
#    rm $1.kdev4
#fi
#
#echo "[Project]
#CreatedFrom=Makefile
#Manager=KDevCustomMakeManager
#Name=$1
#" > $1.kdev4
#
#if [ -d .kdev4 ]
#then
#    rm -rf .kdev4
#fi
#mkdir .kdev4
#
#echo "
#[Launch]
#Launch Configurations=Launch Configuration 0
#
#[Launch][Launch Configuration 0]
#Configured Launch Modes=execute
#Configured Launchers=nativeAppLauncher
#Name=$1 Executable Prog
#Type=Native Application
#
#[Launch][Launch Configuration 0][Data]
#Arguments=
#Dependencies=@Variant(\x00\x00\x00\t\x00\x00\x00\x00\x00)
#Dependency Action=Nothing
#EnvironmentGroup=
#Executable=file:///home/bies/Data/programming/C/SimpleEngine/bin/Debug-linux-x86_64/$1/$1
#External Terminal=konsole --noclose --workdir %workdir -e %exe
#Kill Before Executing Again=4194304
#Project Target=
#Use External Terminal=false
#Working Directory=file:///home/bies/Data/programming/C/SimpleEngine
#isExecutable=true
#" > $1.kdev4

kdevelop

#kdevelop -d gdb bin/Debug-linux-x86_64/$1/$1
