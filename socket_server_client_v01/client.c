/*
 * Socket programming in C - Client side
 * Server node listens on a particular PORT at an IP on the network
 * Reference: https://www.geeksforgeeks.org/socket-programming-cc/
 * Author: Akshat Sinha
 */
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>  // internet address family
#include <string.h>
//#include <netdb.h>  // defines the structure hostent

//#define PORT 8080
#define BUFFER_SIZE 1024

void fv_logErrorEXIT(const char *cptrc_errorMessage,
                           int   pi_socket_fd) {
    //
    // print a system error message on stderr
    perror(cptrc_errorMessage);
    //
    if (pi_socket_fd >= 0) {
        close(pi_socket_fd);
    }
    // cause normal process (unsuccessful) termination
    // abort the program
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    //
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        //
        return EXIT_FAILURE;
    }


    /* [1]
     * create a socket: one endpoint for communication
     * communication domain: specifies implementation-dependent address family,
     *  AF_INET: IPv4 protocol
     * socket type: decides semantics of communication over the socket,
     *  SOCK_STREAM: provides TCP - sequenced, reliable, bidirectional, connection-mode byte streams, and may provide a transmission mechanism for out-of-band data
     * protocol 0: use a default protocol appropriate for the requested socket type
     *  appears on protocol field in the IP header of a packet.
     * returns non-negative socket file descriptor
     */
    printf("\n1. Client creating a socket endpoint for communication using default TCP/IPv4 protocol: ");
    //
    int li_socket_fd = socket(AF_INET,      // internet address domain
                              SOCK_STREAM,  // stream socket type
                              0);           // default TCP protocol
    if (li_socket_fd < 0) {
        fv_logErrorEXIT("ERROR creating socket", li_socket_fd);
    }
    //
    printf("DONE!");

    /*
     * bind the client socket to the port number,
     * specified in addr (custom data structure)
     * Forcefully attaching socket to the port 8080 of the localhost
     */
    printf("\n2. Client binding its socket to port #PORT %s at address ", argv[2]);
    //
    // store server's address, to which to connect to
    struct sockaddr_in lO_addressServer;
    //
    memset(&lO_addressServer,
           '0',
           sizeof(lO_addressServer));
    /*
     bzero((char *) &lO_addressServer,
           sizeof(lO_addressServer));
     */
    //
    lO_addressServer.sin_family = AF_INET;
    /*
     * host-to-network short
     * store numbers in memory in network byte order (big-endian machine)
     * i.e., with the most significant byte, first
     * for proper usage of transmitted packets by network protocols
     */
    lO_addressServer.sin_port = htons(atoi(argv[2]));
    //
    /*
    struct hostent *lptrO_server = gethostbyname(argv[1]);
    if (lptrO_server == NULL) {
        fv_logErrorEXIT("ERROR no such host\n", li_socket_fd);
    }
    //
    bcopy((char *)lptrO_server->h_addr,
          (char *)&lO_addressServer.sin_addr.s_addr,
          lptrO_server->h_length);
    */
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET,
                  argv[1],
                  &lO_addressServer.sin_addr) <= 0) {
        fv_logErrorEXIT("\nInvalid address/ Address not supported \n", li_socket_fd);
    }
    //
    printf("%u  %X: DONE!", lO_addressServer.sin_addr.s_addr, lO_addressServer.sin_addr.s_addr);


    printf("\n3. Client sending connection request to the Server: ");
    //
    // send connection request to the server
    if (connect(li_socket_fd,
                (struct sockaddr *)&lO_addressServer,
                sizeof(lO_addressServer)) < 0) {
        fv_logErrorEXIT("connect", li_socket_fd);
    }
    //
    printf("ESTABLISHed!");

    // connection is established between client and server,
    // they are ready to transfer data
    // both sides can send and receive info


    printf("\nPlease enter the message: ");
    //
    char lc_a1_buffer[BUFFER_SIZE];  // = {0};
    bzero(lc_a1_buffer, BUFFER_SIZE);
    fgets(lc_a1_buffer, BUFFER_SIZE - 1, stdin);
    // char *lptrc_hello = "Hello from client";
    int li_charRead = send(li_socket_fd, lc_a1_buffer, strlen(lc_a1_buffer), 0);
    /*
    int li_n = write(li_socket_fd,
                  lc_a1_buffer,
                  strlen(lc_a1_buffer));
    */
    if (li_charRead < 0) {
        fv_logErrorEXIT("ERROR writing to socket", li_socket_fd);
    }
    //
    printf("Hello message sent\n");


    bzero(lc_a1_buffer, BUFFER_SIZE);
    //
    li_charRead = read(li_socket_fd,
                       lc_a1_buffer,
                       BUFFER_SIZE - 1);
    if (li_charRead < 0) {
        fv_logErrorEXIT("ERROR reading from socket", li_socket_fd);
    }
    //
    printf("[SERVER]: %s\n", lc_a1_buffer);


    close(li_socket_fd);

    return 0;
}
