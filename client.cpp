// ----------------------------------------------------------------------
// Name: Tomas H Woldemichael
// Date: June 6, 2021
// File Name: client.cpp
// Title: PROGRAM 4
// ----------------------------------------------------------------------
// The purpose of this file to act as the client and connect to the server
// and write to the server.
//-------------------------------------------------------------------------
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
#include <chrono>
#include <iostream>
using namespace std;

const int BUFF_SIZE = 1500;

int main(int argc, char *argv[])
{
    // Check if there are enough arguments
    if (argc < 7)
    {
        cout << "NOT ENOUGH ARGUMENTS WERE ENTERED" << endl;
        return -1;
    }

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
        //cout << "Client Socket: " << clientSD << endl;
    }

    freeaddrinfo(result);

    char databuf[nbufs][bufsize];

    // Send over the iterations to the server
    int bytesWritten = write(clientSD, &repetitions, sizeof(repetitions));

    // Start time for test cases and round trip
    auto start = std::chrono::steady_clock::now();

    // Perform the various methods of writing to the server
    for (int i = 0; i < repetitions; i++)
    {
        // Multiple writes
        if (type == 1)
        {
            for (int j = 0; j < nbufs; j++)
            {

                write(clientSD, databuf[j], bufsize);
            }
        }
        // writev
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
        // Single write
        else
        {
            write(clientSD, databuf, (nbufs * bufsize));
        }
    }

    // Test case is now complete
    auto end = std::chrono::steady_clock::now();

    int numOfReads;

    // read the num of reads that the server performed
    read(clientSD, &numOfReads, sizeof(numOfReads));

    // Round trip is now complete
    auto end1 = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::chrono::duration<double> elapsed_seconds_rt = end1 - start;

    double totalBits = nbufs * bufsize * 8 * repetitions;
    double bitsPerSec = totalBits / elapsed_seconds.count();
    double gigaBitsPerSec = bitsPerSec * 1E-9;

    //cout << endl;
    cout << "Test type: " << type << endl;
    cout << "Number of Reads: " << numOfReads << endl;
    cout << "Sending data (Test cases): " << elapsed_seconds.count() * 1E6 << " usec\n";
    cout << "Round Trip (After client reads) " << elapsed_seconds_rt.count() * 1E6 << " usec\n";
    cout << "Throughput: " << gigaBitsPerSec << " GPS \n";
    cout << endl;

    close(clientSD);
}