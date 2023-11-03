#!/bin/bash

# Function to compile
compile() {
    emcc -std=c++17 -IEmpirical/include/ -Os --js-library Empirical/include/emp/web/library_emp.js \
    -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback', '_empDoCppCallback']" \
    -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" \
    -s NO_EXIT_RUNTIME=1 CAAnimate.cpp -o CAAnimate.js
}

# Function to run server in the background
run_server() {
    # Kill any previously running server
    pkill -f "python3 -m http.server"

    # Start server in the background
    python3 -m http.server &
}

# Initial compile and server start
compile
run_server

# Loop to wait for the user to press 'r' to recompile
while true; do
    read -p "Press 'r' and Enter to recompile: " input
    if [[ "$input" == "r" ]]; then
        compile
        echo "Recompilation done."
    fi
done
