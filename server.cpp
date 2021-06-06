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
#include <pthread.h>
#include <thread>
#include <sys/time.h>
#include <iostream>

using namespace std;

const int BUFFSIZE = 1500;
const int NUM_CONNECTIONS = 60;
int repetitions;
int newSD;

void *servicingThread(void *arg)
{

    char dataBuff[BUFFSIZE];

    int bytesRead = read(newSD, &repetitions, sizeof(repetitions));

    int count = 0;
    for (unsigned int i = 0; i < repetitions; i++)
    {
        //cout << "MAKING IT IN LOOP" << endl;
        int readBytes = 0;
        for (unsigned int nRead = 0; nRead < BUFFSIZE; count++)
        {
            //cout << "count: " <<  count << endl;

            readBytes = read(newSD, dataBuff, BUFFSIZE - nRead);

            nRead = nRead + readBytes;
            //cout << readBytes << endl;
        }
        bzero(dataBuff, BUFFSIZE);
        //cout << "PRINT OUT COUNT VALUE: " << count << endl;
        int count = 0;
    }

    // Send the number of read( ) calls made, (i.e., count in the above) as an acknowledgment.
    write(newSD, &count, sizeof(count));

    // End session and exit
    close(newSD);

    //exit(0);
    return nullptr;
}

int main(int argc, char *argv[])
{
    // Error check if enough arguments have been passed in
    // if (argc < 2)
    // {
    //     cout << "NOT ENOUGH ARGUMENTS" << endl;

    //     return -1;
    // }

    char *serverPort = argv[1];
    //repetitions = atoi(argv[2]);

    // build the recving socket
    sockaddr_in acceptSocketAddress;
    bzero((char *)&acceptSocketAddress, sizeof(acceptSocketAddress));
    acceptSocketAddress.sin_family = AF_INET;
    acceptSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    acceptSocketAddress.sin_port = htons(atoi(serverPort));

    //Open the socket and bind
    int serverSD = socket(AF_INET, SOCK_STREAM, 0);
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
    listen(serverSD, NUM_CONNECTIONS);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    int numOfConnections = 0;
    while (numOfConnections < NUM_CONNECTIONS)
    {
        newSD = accept(serverSD, (sockaddr *)&newSockAddr, &newSockAddrSize);
        cout << "Accepted Socket #: " << newSD << endl;

        pthread_t serverHelperThread;
        pthread_create(&serverHelperThread, NULL, servicingThread, NULL);
       
        //pthread_join(serverHelperThread, NULL);
        numOfConnections = numOfConnections + 1;
        pthread_detach(serverHelperThread);
    }

    sleep(1);
    close(serverSD);
}