#!/bin/sh

if [ -f "calculator" ]; then
    echo "your output will be placed at *.res"
else
    echo "please compile calculator first"
    exit 0
fi

for file in $(ls *.txt); do
    base=$(basename $file)
    name=${base%.*}
    ./calculator $file > $name.res
    
    echo "testing $name... "
    diff $name.out $name.res
    echo "========================"
done
