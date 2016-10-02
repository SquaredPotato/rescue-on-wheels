
#ifndef COLUMN_SERVER_H
#define COLUMN_SERVER_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <math.h>

class Server {
public:
    Server(int port);

    bool bindPort();
    bool acceptClient(void (*cb)(int clientSocket));
    void handlePackets(int socket, void (*cb)(char * data, size_t size), void (*scb)());
    void writePacket(int socket, void *data, size_t size);
    void close();

private:
    int handle;
    size_t bufferSize = 200;
    sockaddr_in server;
    sockaddr_in client;
};


#endif //COLUMN_SERVER_H
