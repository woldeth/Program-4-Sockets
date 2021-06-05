// Tomas Woldemichael
// June 1st, 2021
// Program 4

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

const int BUFF_SIZE = 1500;

int main(int argc, char *argv[])
{

    // if (argc < 7)
    // {
    //     cout << "NOT ENOUGH ARGUMENTS WERE ENTERED" << endl;
    //     return -1;
    // }

    // ./

    // ./client csslab11 5002 20000 10 150 1
    char *serverName = argv[1];
    char *serverPort = argv[2];
    int repetitions = atoi(argv[3]);
    int nbufs = atoi(argv[4]);
    int bufsize = atoi(argv[5]);
    int type = atoi(argv[6]);

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int clientSD = -1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;                                   /* Allow IPv4 or IPv6*/
    hints.ai_socktype = SOCK_STREAM;                               /* TCP */
    hints.ai_flags = 0;                                            /* Optional Options*/
    hints.ai_protocol = 0;                                         /* Allow any protocol*/
    int rc = getaddrinfo(serverName, serverPort, &hints, &result); // replacement for get host by name

    if (rc != 0)
    {
        cerr << "ERROR: " << gai_strerror(rc) << endl;
        exit(EXIT_FAILURE);
    }

    /*
     * Iterate through addresses and connect
     */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        clientSD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (clientSD == -1)
        {
            continue;
        }
        /*
		* A socket has been successfully created
		*/
        rc = connect(clientSD, rp->ai_addr, rp->ai_addrlen);
        if (rc < 0)
        {
            cerr << "Connection Failed" << endl;
            close(clientSD);
            return -1;
        }
        else //success
        {
            break;
        }
    }

    if (rp == NULL)
    {
        cerr << "No valid address" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Client Socket: " << clientSD << endl;
    }

    freeaddrinfo(result);

    // Data buffer for the write and the reads
    char databuf[nbufs][bufsize];

    // Data writing to server using specified type of transfer
    for (int i = 0; i < repetitions; i++)
    {

        // Multiple writes:
        if (type == 1)
        {
            for (int j = 0; j < nbufs; j++)
            {
                //cout << "PEFORMING TEST CASES" << endl;
                write(clientSD, databuf[j], bufsize);
            }
        }
        // writev:
        else if (type == 2)
        {
            struct iovec vector[nbufs];
            for (int j = 0; j < nbufs; j++)
            {
                vector[j].iov_base = databuf[j];
                vector[j].iov_len = bufsize;
            }
            writev(clientSD, vector, nbufs);
        }
        // Single write:
        else
        {
            write(clientSD, databuf, (nbufs * bufsize));
        }
    }

    // Reveive the acknowledgment abd quantity from the server for how many
    // time it called read()
    int numOfReads;
    read(clientSD, &numOfReads, sizeof(numOfReads));

    cout << "NUMOFREADS " << numOfReads << endl;

    // End session and exit
    close(clientSD);
}