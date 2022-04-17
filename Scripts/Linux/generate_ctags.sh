#!/bin/bash

cd ../..
#ctags -e -R .
ctags -e -R --languages=C,C++

#ctags --langmap=c:+.h --languages=c,c++ -R ~/1.3.204.1/source/Vulkan-Headers/include/vulkan/
