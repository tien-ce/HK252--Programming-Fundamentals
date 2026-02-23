#!/bin/bash
# Default compilation command
COMPILE_CMD="g++ -o main main.cpp water_seven.cpp -I. -std=c++11"

# Check if the user wants to enable DEBUG mode
if [ "$1" == "-d" ]; then
    echo "Enabling DEBUG mode..."
    COMPILE_CMD="$COMPILE_CMD -DDEBUG"
fi
# 1. Compile the program
echo "Compiling..."
$COMPILE_CMD

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# 2. Clear previous results
> results.log

# 3. Run all test cases
echo "Running tests..."
for file in input/tc_*.txt; do
    echo "Testing $file..." >> results.log
    ./main "$file" >> results.log
    echo -e "\n----------------------------\n" >> results.log
done

echo "Done. Check results.log for output."


