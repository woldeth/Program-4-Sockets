

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;                                   /* Allow IPv4 or IPv6*/
    hints.ai_socktype = SOCK_STREAM;                               /* TCP */
    hints.ai_flags = 0;                                            /* Optional Options*/
    hints.ai_protocol = 0;  