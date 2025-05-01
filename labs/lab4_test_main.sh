#!/bin/bash

passed=0
failed=0

echo "start building main program:"
echo "compiling to assembly lines..."
cc main.c -S
cc increment.c -S
echo "translating to opcodes..."
cc main.s -c
cc increment.s -c
echo "statically linking all required opcodes..."
cc main.o increment.o -o main
echo "build successfully done!"

while IFS=',' read -r input correct_output
do
	program_output=$(./main <<< "$input")
	correct_output=$(echo $correct_output | xargs)

	if [[ "$program_output" == "$correct_output" ]]; 
	then
		result="passed"
		((passed++))
	else
		result="failed"
		((failed++))
	fi
	echo "input: $input, main: $program_output, correct: $correct_output ==> $result"

done < "./test_inputs.txt"

echo ""
echo "total passed: $passed"
echo "total failed: $failed"
