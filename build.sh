mkdir -p build
gcc -Wall -std=c99 ./demos/request.c -o ./build/request
gcc -Wall -std=c99 ./demos/server.c -o ./build/server
