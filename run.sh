mkdir -p build
gcc -Wall -std=c99 ./demos/request.c -o ./build/req
gcc -Wall -std=c99 ./demos/server.c -o ./build/server
