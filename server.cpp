#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

const int BUFFSIZE = 1500;
const int NUM_CONNECTIONS = 5;

int serverPort;
int repetitions;
int serverSD;
int newSD;

int main(int argc, char *argv[])
{
    // Error check if enough arguments have been passed in
    // if (argc < 3)
    // {
    //     cout << "NOT ENOUGH ARGUMENTS" << endl;
   
    //     return -1;
    // }


    char serverPort[6] = "12345";
    // char serverPort = atoi(argv[1]);



}