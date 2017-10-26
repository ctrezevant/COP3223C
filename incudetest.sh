# Test script to retrieve paths of all the system's include directories (for header files)
INCLUDE_DIRS=`echo | gcc -E -Wp,-v - 2>&1 >/dev/null | grep -vE 'ignoring|End of search list.|#include|version|\(|\)' | grep '/'`
CPPCHECK_INCLUDE_FLAGS=""

for DIR in $INCLUDE_DIRS;
do
  CPPCHECK_INCLUDE_FLAGS=${CPPCHECK_INCLUDE_FLAGS}" -I $DIR"
done

echo $CPPCHECK_INCLUDE_FLAGS
