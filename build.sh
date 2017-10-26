#!/usr/bin/env bash

# Travis Automatic Build Script

# Find subdirectories with makefiles (which we can then test)
BUILDABLE_DIRS=`find . -name '*akefile' | grep -v './Template' | awk '{gsub(/[^\/]*$/,"");print}'`

for SUBDIR in $BUILDABLE_DIRS;
do

cd $SUBDIR

MAKEFILE=`cat *akefile`

echo $SUBDIR

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

cd ..

done

unset BUILDABLE_DIRS
unset MAKEFILE
