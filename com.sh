#!/bin/bash
gcc -g -o servercallback ./server_callback.cpp -lstdc++
echo server compiled
gcc -g -o clientcallback ./client_callback.cpp -lstdc++
echo client compiled
