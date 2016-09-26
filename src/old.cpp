#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <math.h>

int fd;

void Motorinit() {
    uint8_t Totalpower[2] = {4, 230};     // power between 0 and 255
    uint8_t Softstart[3] = {0x91, 23, 0};  // add explanation

    pinMode(2, OUTPUT);

    wiringPiSetup();
    pullUpDnControl(0, PUD_DOWN);

    fd = wiringPiI2CSetup(0x32);

    write(fd, &Totalpower[0], 2);
    write(fd, &Softstart[0], 3);

    //number of bytes = 3
    //What is a soft start?
}

void runMotors(int rotation, double speed) {
    double ls = speed, rs = speed;
    bool ld = true, rd = true;

    if (rotation > -180 && rotation <= -135) {
        double d = (rotation * -1. - 135.) / 45.;
        rs = rs * d;
        ld = false;
        rd = false;
    } else if (rotation > -135 && rotation <= -90) {
        double d = fabs((rotation * -1. - 90.) / 45. - 1.);
        rs = rs * d;

        ld = false;
        rd = true;
    } else if (rotation > -90 && rotation <= -45.) {
        double d = (rotation * -1. - 45.) / 45.;
        ls = ls * d;

        ld = false;
        rd = true;
    } else if (rotation > -45 && rotation <= 0) {
        double d = fabs((rotation * -1.) / 45. - 1.);
        ls = ls * d;

        ld = true;
        rd = true;
    } else if (rotation > 0 && rotation <= 45.) {
        double d = fabs(rotation / 45. - 1.);
        rs = rs * d;

        ld = true;
        rd = true;
    } else if (rotation > 45 && rotation <= 90) {
        double d = (rotation - 45.) / 45.;
        rs = rs * d;

        ld = true;
        rd = false;
    } else if (rotation > 90 && rotation <= 135) {
        double d = fabs((rotation - 90.) / 45. - 1.);
        ls = ls * d;

        ld = true;
        rd = false;
    } else if (rotation > 135 && rotation <= 180) {
        double d = (rotation - 135.) / 45.;
        ls = ls * d;

        ld = false;
        rd = false;
    }

    int p1H, p2H, p1L, p2L, d1, d2;

    //convert to hexadecimal with high and low significant byte
    p2H = ((int) rs >> 8);
    p2L = ((int) rs & 0xFF);

    p1H = ((int) ls >> 8);
    p1L = ((int) ls & 0xFF);

    //just to be sure, convert bool to char (int)
    if (ld) { d1 = 2; } else { d1 = 1; }
    if (rd) { d2 = 2; } else { d2 = 1; }

    uint8_t MotorArray[7] = {7, (uint8_t) p1H, (uint8_t) p1L, (uint8_t) d1, (uint8_t) p2H, (uint8_t) p2L, (uint8_t) d2};

    write(fd, &MotorArray[0], 7);
}

int main2(int argc, char *argv[]) {
    Motorinit();

    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[20];

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1337);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);
    usleep(100000);
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);


    //Receive a message from client
    while ((read_size = (int) recv(client_sock, client_message, 20, 0)) > 0) {
        char *token = strtok(client_message, ",");

        int angle = atoi(token);
        token = strtok(NULL, ",");
        int speed = atoi(token);

//        printf("Angle: %d, Speed: %d \n", angle, speed);

        runMotors(angle, sqrt(speed) * 93);

        client_message[0] = '\0';
    }

    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    runMotors(0, 0);

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    return 0;
}
