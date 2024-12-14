#! /usr/sh

FILES=$(find . -not -path "./third_party/*" -not -path "./build/*" \( -name '*.cc' -o -name '*.c' -o -name '*.h' \))

for file in $FILES; do
  clang-format -i -style=file $file
done
echo "Clang-format run successful"
