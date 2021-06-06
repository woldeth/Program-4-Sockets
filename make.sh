#!/bin/sh
g++ client.cpp -O3 -o client
g++ server.cpp -lpthread -o server

chmod 700 client
chmod 700 server

