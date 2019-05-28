# Inter-Process Communication between a Client and a Server through Sockets


## Single Connection-oriented Server:

### Limitations
⋅⋅* Server handles *only* one connection and, then, dies!


## Concurrent Connection-oriented Server:

### Features
⋅⋅* Server runs *indefinitely*
⋅⋅* Server can handle multiple connections, *simultaneously*
⋅⋅⋅ each new connection forks off its own new process


Execute the following commands on Linux shell terminal

## 1. One time compilation of C programs
**NOTE**: gcc version at my end was 5.4.0 20160609
```shell
$ gcc server_connectionOrientedConcurrent.c -o server
$ gcc client.c -o client
```
## 2. First start server with any port number (preferably > 2000)
```shell
$ ./server 8081
```

## 3. Then, in a NEW Linux shell terminal run the client with IP address and port number of the server
```shell
$ ./client 127.0.0.1 8081
```

Example output:

![alt text](https://github.com/engrvivs/c-ipc/blob/master/socket_server_client_v01/TCPIP_ClientServer_v01.png "Example output")
