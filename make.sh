#!/bin/sh
g++ client.cpp -o client
g++ server.cpp -lpthread -o server

chmod 700 client
chmod 700 server

