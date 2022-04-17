#!/bin/bash


oclint -report-type html -o report.html Application/Application.c Utils/SimpleStandardLibrary.c Sandbox.c -I../../../Engine/src -I../../../SandboxApp/src -c
