/*
 * Socket programming in C - Server side
 * Server node listens on a particular PORT at an IP on the network
 * Server in the Internet domain using TCP/IPv4 protocol
 * The port number is passed as an argument
 * References:
 *  https://www.geeksforgeeks.org/socket-programming-cc/
 *  https://www.linuxhowtos.org/C_C++/socket.htm
 * Author: Akshat Sinha
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <sys/types.h>  // definitions of data types, used in system calls by sys/socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets
#include <netinet/in.h>  // constants and structures needed for internet domain addresses

// #define PORT 8080
#define BUFFER_SIZE 1024

void fv_logErrorEXIT(const char *cptrc_errorMessage,
                           int   pi_socketConn_fd,
                           int   pi_socketRW_fd) {
    //
    // print a system error message on stderr
    perror(cptrc_errorMessage);
    //
    if (pi_socketConn_fd >= 0) {
        close(pi_socketConn_fd);
    }
    //
    if (pi_socketRW_fd >= 0) {
        close(pi_socketRW_fd);
    }
    // cause normal process (unsuccessful) termination
    // abort the program
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[]) {
    //
    if (argc < 2) {
        //
        fprintf(stderr, "Usage %s port\n", argv[0]);
        //
        return EXIT_FAILURE;
    }


    /* [1]
     * create a new socket: one endpoint for communication
     * communication domain: specifies implementation-dependent address family,
     *  AF_INET: IPv4 protocol
     * socket type: decides semantics of communication over the socket,
     *  SOCK_STREAM: provides TCP - sequenced, reliable, bidirectional, connection-mode byte streams, and may provide a transmission mechanism for out-of-band data
     * protocol 0: use a default protocol appropriate for the requested socket type
     *  appears on protocol field in the IP header of a packet.
     * returns non-negative socket file descriptor (i.e., array subscript into the file descriptor table)
     */
    printf("\n1. Server creating a new socket endpoint\nfor communication using default TCP/IPv4 protocol with socket id");
    //
    int li_socketConn_fd = socket(AF_INET,      // internet address domain
                                  SOCK_STREAM,  // stream socket type
                                  0),           // default TCP protocol
        li_socketRW_fd = -1;
    //
    if (li_socketConn_fd < 0) {
        fv_logErrorEXIT("socket failed", li_socketConn_fd, li_socketRW_fd);
    }
    //
    printf(" %d: DONE!", li_socketConn_fd);


    /* [1.5]
     * <optional> 
     * to manipulate socket's options
     * allows reuse of address and port
     * prevents error, like, “address already in use”
     * Forcefully attaching socket to the port 8080 
     */
    printf("\n2. Manipulating the socket's options: ");
    //
    int li_socket_optionValue = 1;
    //
    if (setsockopt(li_socketConn_fd,
                   SOL_SOCKET,  // level
                   SO_REUSEADDR | SO_REUSEPORT,  // optname
                   &li_socket_optionValue,        // optval
                   sizeof(li_socket_optionValue))) {
        fv_logErrorEXIT("setsockopt", li_socketConn_fd, li_socketRW_fd);
    }
    //
    printf("DONE!");


    /* [2]
     * bind the server socket to the current host's address and given port number, on which the server will run
     * specified in addr (custom data structure)
     */
    printf("\n3. Binding the server socket to the port (%s) at address ", argv[1]);
    //
    // structure to store server's internet address information
    struct sockaddr_in lO_addressServer;    // 16 bytes long
    // set all values in the buffer to 0
    bzero((char *) &lO_addressServer,   // pointer to the buffer
          sizeof(lO_addressServer));    // size of the buffer
    //
    // code for the address family
    lO_addressServer.sin_family = AF_INET;
    // IP address of the host machine on which the server is running
    lO_addressServer.sin_addr.s_addr = INADDR_ANY;
    /*
     * host-to-network short
     * store numbers in memory in network byte order (BIG-endian machine)
     * i.e., with the most significant byte, first
     * for proper usage of transmitted packets by network protocols
     */
    // port number on which the server will listen for and accept connections
    lO_addressServer.sin_port = htons(atoi(argv[1]));
    //
    //
    if (bind(li_socketConn_fd,
            (struct sockaddr *) &lO_addressServer,
             sizeof(lO_addressServer)) < 0) {
        fv_logErrorEXIT("bind failed", li_socketConn_fd, li_socketRW_fd);
    }
    //
    printf("%u  %X: DONE!", lO_addressServer.sin_addr.s_addr, lO_addressServer.sin_addr.s_addr);
    //
    printf("\nNetwork port #: %u %X\n",
            lO_addressServer.sin_port,
            lO_addressServer.sin_port);


    /* [3]
     * Put the server socket in a passive mode,
     * it waits for the client to approach the server to make a connection
     * The backlog queue size (3), defines the maximum length to which the queue of 
     * pending connections for li_socketConn_fd may grow
     * If a connection request arrives when the queue is full,
     * the client may receive an error with an indication of ECONNREFUSED
     */
    printf("\n4. Server socket is listening...");
    //
    // allows the process to listen on the socket for connections
    // number of connections that can be waiting, while the process is handling a particular connection
    if (listen(li_socketConn_fd, 3) < 0) {
        fv_logErrorEXIT("listen", li_socketConn_fd, li_socketRW_fd);
    }


    /* [4]
     * Extract the first connection request on the queue of pending connections for the listening socket, li_socketConn_fd
     * Returns a file descriptor referring to a newly created connected socket
     */
    printf("\n5. Accepting a NEW connection: ");
    //
    // address of the client that connects to the server
    struct sockaddr_in lO_addressClient;
    //
    // to store size of the client's address
    socklen_t lui_sizeClient = sizeof(lO_addressClient);
    //
    // block the process untill a client connects to the server
    // the process wakes up when a connection from a client is successfully established
    li_socketRW_fd = accept(li_socketConn_fd,
                            (struct sockaddr *) &lO_addressClient,
                            &lui_sizeClient);
    if (li_socketRW_fd < 0) {
        fv_logErrorEXIT("accept", li_socketConn_fd, li_socketRW_fd);
    }
    //
    printf("Successful!\n");


    printf("\nDEBUG: Connecting Client's Internet Address Info");
    printf("\nAddress: %u %X",
            lO_addressClient.sin_addr.s_addr,
            lO_addressClient.sin_addr.s_addr);
    printf("\nNetwork port #: %u %X",
            lO_addressClient.sin_port,
            lO_addressClient.sin_port);
    printf("\nHost port #: %u %X\n",
            ntohs(lO_addressClient.sin_port),
            ntohs(lO_addressClient.sin_port));

    // connection is established between client and server,
    // they are ready to transfer data
    // both sides can send and receive info


    // server reads character from the socket connection into this buffer
    char lc_a1_buffer[BUFFER_SIZE] = {0};
    // initialize buffer to 0
    // bzero(lc_a1_buffer, BUFFER_SIZE);
    // read message from the socket connection
    // li_n: number of characters read
    // NOTE: read() will block until there is something for it to read in the socket,
    //       i.e., after the client has executed a write()
    int li_n = read(li_socketRW_fd, lc_a1_buffer, BUFFER_SIZE - 1);
    //
    if (li_n < 0) {
        fv_logErrorEXIT("ERROR reading from socket",
                        li_socketConn_fd, li_socketRW_fd);
    }
    //
    printf("[Client]: %s\n", lc_a1_buffer);

    // write message to the client
    // li_n: number of characters written
    // last argument: size of the message
    char *lptrc_acknowledge = "I received your message.";
    // li_n = write(li_socketRW_fd, lptrc_acknowledge, strlen(lptrc_acknowledge));
    li_n = send(li_socketRW_fd, lptrc_acknowledge, strlen(lptrc_acknowledge), 0);
    //
    if (li_n < 0) {
        fv_logErrorEXIT("ERROR writing to socket",
                        li_socketConn_fd, li_socketRW_fd);
    }
    //
    printf("Acknowledgement message sent\n");


    close(li_socketRW_fd);
    close(li_socketConn_fd);


    return 0;
}
