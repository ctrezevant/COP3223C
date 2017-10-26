#!/usr/bin/env bash

# Travis Automatic Build Script

BLACKLIST="./Template"

# Find subdirectories with makefiles (which we can then test), excluding the blacklist
BUILDABLE_DIRS=`find . -name '*akefile' | grep -v $BLACKLIST | awk '{gsub(/[^\/]*$/,"");print}'`

for SUBDIR in $BUILDABLE_DIRS;
do

cd $SUBDIR

MAKEFILE=`cat *akefile`

echo -e "\n/////////////////////////////////////////////////////////\n"
echo -e "\t ** NOW BUILDING: $SUBDIR **"

  TESTED=0
  # Run make test, if rule exists
  if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^test[\t ]*:/ {print}')" != "" ]
  then
    echo -e "\n>>> Running: make test"
    make test
    TESTED=1
    echo -e ">>> FINISHED: make test"
  fi

  # run make all, but only if the test rule isn't defined.
  # By default make test will build the project and run tests, as well, so
  # we don't need to build the project twice (once for all, again for tests)
  if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^all[\t ]*:/ {print}')" != "" ] && [ $TESTED == 0 ]
  then
    echo -e "\n>>> Running: make all"
    make
    echo -e ">>> FINISHED: make all"
  fi

  # Run make clean, if rule exists
  if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^clean[\t ]*:/ {print}')" != "" ]
  then
    echo -e "\n>>> Running: make clean"
    make clean
    echo -e ">>> FINISHED: make clean"
  fi

echo -e "\n/////////////////////////////////////////////////////////\n"

cd ..

done

unset BUILDABLE_DIRS
unset MAKEFILE
