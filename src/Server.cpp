
#include "Server.h"

Server::Server(int port) {

}

bool Server::bindPort() {
    //Create socket
    this->handle = socket(AF_INET, SOCK_STREAM, 0);
    if (this->handle == -1) {
        perror("Could not create socket");
        return false;
    }

    //Prepare the sockaddr_in structure
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(1337);

    //Bind
    if (bind(this->handle, (struct sockaddr *) &this->server, sizeof(this->server)) < 0) {
        return false;
    }

    //Listen
    listen(this->handle, 3);

    return true;
}

bool Server::acceptClient(void (*cb)(int clientSocket)) {
    int c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    int clientSocket = accept(this->handle, (struct sockaddr *) &this->client, (socklen_t *) &c);
    if (clientSocket < 0) {
        perror("accept failed");
        return false;
    }

    (*cb)(clientSocket);

    return true;
}

void Server::handlePackets(int socket, void (*cb)(char *data, size_t size), void (*scb)()) {
    char *clientMessage;
    int read_size;

    clientMessage = (char *) malloc(this->bufferSize);

    while ((read_size = (int) recv(socket, clientMessage, this->bufferSize, 0)) > 0) {
        (*cb)(clientMessage, this->bufferSize);
    }

    (*scb)();

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    this->close();
}

void Server::close() {

}

void Server::writePacket(int socket, void *data, size_t size) {
    write(socket, data, size);
}
