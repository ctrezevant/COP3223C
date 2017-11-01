#!/usr/bin/env bash

# Travis CI Build Script - COP3223C
# (c) 2017 Charlton Trezevant
# MIT License
# https://github.com/ctrezevant/COP3223C

# Set to 1 to enable static code analysis before building (via scan-build)
RUN_LINTER=1

# List of subdirectories to blacklist (in our case, never build the Template project)
BLACKLIST="./Template"

# How many commits back should we check for changes to files?
COMMIT_RANGE_LBOUND=10

# Find subdirectories with new changes and makefiles (which we can then build and test), excluding the blacklist
# CHANGED_DIRS will collect the list of new directories that contain new changes (in the past $COMMIT_RANGE_LBOUND commits) to .c source files
# This will ensure that Travis attempts to build only the latest changes, which is what we want
CHANGED_DIRS=`git diff --name-only HEAD~$COMMIT_RANGE_LBOUND..HEAD '*.c' | cut -d '/' -f1 | sed -e 's/^/.\//'`

# BUILDABLE_DIRS will contain the list of directories with new/changed files that *also* have makefiles (and can therefore be built)
BUILDABLE_DIRS=`find $CHANGED_DIRS -name '*akefile' | grep -v $BLACKLIST | awk '{gsub(/[^\/]*$/,"");print}'`

# We'll keep track of the number of build failures here
FAILED_BUILDS=""
for SUBDIR in $BUILDABLE_DIRS;
do

  cd $SUBDIR

  MAKEFILE=`cat *akefile`

  echo -e "\n/////////////////////////////////////////////////////////\n"
  echo -e "\t *** NOW BUILDING: $SUBDIR ***"

    # Run scan-build, for static analysis
    # This will analyze code for any syntactical errors etc, using gcc
    # In the future, you may also prepend scan-build to make, which will run
    # scan-build while simultaneously building your project
    if [ $RUN_LINTER == 1 ]
    then
      echo -e "\n>>> Running: static analysis (scan-build)"
      scan-build gcc -std=c99 -c src/*
      LINT_PASS=$?
      echo -e ">>> FINISHED: static analysis (scan-build)"
    fi

    TESTED=0
    # Run make test, if rule exists
    if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^test[\t ]*:/ {print}')" != "" ]
    then
      echo -e "\n>>> Running: make test"
      make test
      MAKE_PASS=$?
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
      MAKE_PASS=$?
      echo -e ">>> FINISHED: make all"
    fi

    # Run make clean, if rule exists
    if [ "$(echo "$MAKEFILE" | awk '$0 ~ /^clean[\t ]*:/ {print}')" != "" ]
    then
      echo -e "\n>>> Running: make clean"
      make clean
      echo -e ">>> FINISHED: make clean"
    fi

    if [ $(($LINT_PASS + $MAKE_PASS)) == 0 ]
    then
        echo -e "\n\t *** BUILD SUCCESS: $SUBDIR ***"
    else
        echo -e "\n\t !!! BUILD FAILURE: $SUBDIR !!!"
        FAILED_BUILDS=${FAILED_BUILDS}" $SUBDIR"
    fi

  echo -e "\n/////////////////////////////////////////////////////////\n"

  unset MAKEFILE
  unset LINT_PASS
  unset MAKE_PASS

  cd ..

done

# Finally, display a summary of all builds performed, and how successful those were.
# If any of our builds failed, we'll return the appropriate status code so Travis will know
echo -e "\n---------------REPORT---------------\n"
if [ -z $FAILED_BUILDS ]
then
  echo -e "All projects built successfully!"
  EXCODE=0
else
  echo -e "Some projects failed to build :("
  echo -e "Projects with build failures: $FAILED_BUILDS"
  EXCODE=1
fi
echo -e "\n------------------------------------\n"

# Unset any variables we created in the script
unset RUN_LINTER
unset BLACKLIST
unset COMMIT_RANGE_LBOUND
unset CHANGED_DIRS
unset BUILDABLE_DIRS
unset FAILED_BUILDS

exit $EXCODE
