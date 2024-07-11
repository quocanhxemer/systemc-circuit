gcc -o main.o main.c
if [ $? -ne 0 ] ; then
  echo "Compilation failed"
  exit 1
fi

#Test should fail
tests=( #Fehlt arguments
        "--cycles" "./main.o: option '--cycles' requires an argument" 1
        "-c" "./main.o: option requires an argument -- 'c'" 1
        "--cacheline-size" "./main.o: option '--cacheline-size' requires an argument" 1
        "--l1-lines" "./main.o: option '--l1-lines' requires an argument" 1
        "--l2-lines" "./main.o: option '--l2-lines' requires an argument" 1
        "--l1-latency" "./main.o: option '--l1-latency' requires an argument" 1
        "--l2-latency" "./main.o: option '--l2-latency' requires an argument" 1

        #Integer overflow
        "--memory-latency" "./main.o: option '--memory-latency' requires an argument" 1
        "--cycles -2147483649" "Invalid number in cycles: -2147483649 must be positiv" 1
        "-c -2147483649" "Invalid number in cycles: -2147483649 must be positiv" 1
        "--cacheline-size -1" "Invalid number in cacheline-size: -1 over- or underflow unsigned integer" 1
        "--l1-lines -1" "Invalid number in l1-lines: -1 over- or underflow unsigned integer" 1
        "--l2-lines -1" "Invalid number in l2-lines: -1 over- or underflow unsigned integer" 1
        "--l1-latency -1" "Invalid number in l1-latency: -1 over- or underflow unsigned integer" 1
        "--l2-latency -1" "Invalid number in l2-latency: -1 over- or underflow unsigned integer" 1
        "--memory-latency -1" "Invalid number in memory-latency: -1 over- or underflow unsigned integer" 1
)


#Run Tests

#Nur Color for Output
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'
#testfailedcases
failed=0
for ((i = 0; i < ${#tests[@]}; i+=3)); do
    input="${tests[i]}"
    expected_output="${tests[i+1]}"
    expected_exit_status="${tests[i+2]}"

# Execute the program with the test input
    output=$(./main.o $input 2>&1)
    exit_status=$?
   if [[ "$output" == *"$expected_output"* && $exit_status -eq $expected_exit_status ]]; then
          echo -e "${GREEN}Test passed: $input"
      else
          failed=$((failed + 1))
          echo -e "${RESET}********************************************************************"
          echo -e "${RED}    Test failed: $input"
          echo -e "${GREEN}    Expected output: $expected_output"
          echo -e "${RED}    Got output: $output"
          echo -e "${GREEN}    Expected exit status: $expected_exit_status"
          echo -e "${RED}    Got exit status: $exit_status"
          echo -e "${RESET}********************************************************************"
      fi
  done
if [ $failed -eq 0 ]; then
  echo ""
  echo -e "*****************SUCCEED***********************"
else
    echo -e "${RED}$failed Test failed"
fi


