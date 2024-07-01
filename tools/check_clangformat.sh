#! /usr/sh

FILES=$(find . -not -path "./third_party/*" -not -path "./build/*" \( -name '*.cc' -o -name '*.c' -o -name '*.h' \))
TMPFILE="./formatted_file"

for file in $FILES; do
  clang-format -style=file $file > $TMPFILE
  if ! diff $file $TMPFILE > /dev/null; then
    echo "Clang-format failed on $file"
    rm $TMPFILE
    exit 1
  fi 
  rm $TMPFILE
done
echo "Clang-format check successful"
