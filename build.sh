#!/usr/bin/env bash

# Travis Automatic Build Script

BLACKLIST="./Template"

# Find subdirectories with makefiles (which we can then test), excluding the blacklist
BUILDABLE_DIRS=`find . -name '*akefile' | grep -v $BLACKLIST | awk '{gsub(/[^\/]*$/,"");print}'`

for SUBDIR in $BUILDABLE_DIRS;
do

cd $SUBDIR

MAKEFILE=`cat *akefile`

echo "---------------------------------------------------------"
echo -e "\t ** NOW BUILDING: $SUBDIR **"

  # Run make clean, if rule exists
  if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^all[\t ]*:/ {print}')" != "" ]
  then
    make
  fi

  # Run make test, if rule exists
  if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^test[\t ]*:/ {print}')" != "" ]
  then
    make test
  fi

  # Run make clean, if rule exists
  if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^clean[\t ]*:/ {print}')" != "" ]
  then
    make clean
  fi

echo "---------------------------------------------------------"

cd ..

done

unset BUILDABLE_DIRS
unset MAKEFILE
