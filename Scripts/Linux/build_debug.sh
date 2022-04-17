#!/bin/bash

cd ../..

Scripts/Linux/copy_files.sh "$1"

startTime=$(date +%s%3N)
make config=debug
buildResult=$?
echo "Result: $buildResult"
endTime=$(date +%s%3N)
diffTime=$(( $endTime - $startTime ))

echo "Builds $1 in $diffTime ms"

if [ $buildResult -eq 0 ]
then
    pwd
    bin/Debug-linux-x86_64/$1/$1
else
    echo "Failed to build project"
fi
