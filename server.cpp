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
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>

using namespace std;

const int BUFFSIZE = 1500;
const int NUM_CONNECTIONS = 5;
int serverSD;

void servicingThread(int signal)
{
    cout << "Tomas2 " << endl;
    // Data buffer for receiving data from client
    char dataBuff[BUFFSIZE];
    int repetitions;
    int newSD;

    // Repeating the read() function for reading from the client and
    // keeping track of the number or total reads.
    int count = 0;
    for (int i = 0; i < repetitions; i++)
    {
        for (int nRead = 0; (nRead += read(newSD, dataBuff, BUFFSIZE - nRead)) < BUFFSIZE; ++count)
            ;
    }

    // Send the number of read( ) calls made, (i.e., count in the above) as an acknowledgment.
    write(newSD, &count, sizeof(count));

    // End session and exit
    close(newSD);
    close(serverSD);
    exit(0);
}

int main(int argc, char *argv[])
{
    // Error check if enough arguments have been passed in
    // if (argc < 2)
    // {
    //     cout << "NOT ENOUGH ARGUMENTS" << endl;

    //     return -1;
    // }

    // --------------------- COMPILE CODE --------------------------- ;
    // ./server csslab11.uwb.edu 20000
    char *serverPort = argv[1];
    int repetitions = atoi(argv[2]);

    char *serverName;
    char databuf[BUFFSIZE];
    bzero(databuf, BUFFSIZE);

    // build the recving socket
    sockaddr_in acceptSocketAddress;
    bzero((char *)&acceptSocketAddress, sizeof(acceptSocketAddress));
    acceptSocketAddress.sin_family = AF_INET;
    acceptSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    acceptSocketAddress.sin_port = htons(atoi(serverPort));

    //Open the socket and bind
    serverSD = socket(AF_INET, SOCK_STREAM, 0);
    const int on = 1;
    setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int));
    cout << "Socket #: " << serverSD << endl;

    int rc = bind(serverSD, (sockaddr *)&acceptSocketAddress, sizeof(acceptSocketAddress));
    if (rc < 0)
    {
        cerr << "Bind Failed" << endl;
        close(serverSD);
    }

    // Listen and accept
    listen(serverSD, NUM_CONNECTIONS); //setting number of pending connections
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    int newSD = accept(serverSD, (sockaddr *)&newSockAddr, &newSockAddrSize);
    cout << "Accepted Socket #: " << newSD << endl;

    // Wait for the I/O interrupt signal and change the new socket into an asynchronous connection
    cout << "Tomas1 " << endl;
    signal(SIGIO, servicingThread);
    fcntl(newSD, F_SETOWN, getpid());
    fcntl(newSD, F_SETFL, FASYNC);
    cout << "Tomas3 " << endl;
}