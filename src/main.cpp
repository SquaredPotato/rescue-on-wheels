#include <wiringPi.h>
#include "Server.h"
#include "Motors.h"

Motors *motors;
Server *server;

void onClose() {
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    motors->calculateAndSend(0, 0);
}

void onPacket(char *data, size_t size) {
    int angle;
    int speed;
    int buzz;

    sscanf(data, "%d|%d|%d ", &angle, &speed, &buzz);

    motors->calculateAndSend(angle, sqrt(speed) * 100);

    digitalWrite(2, buzz);
}

void onClient(int client) {
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);
    usleep(100000);
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    server->handlePackets(client, onPacket, onClose);
}

int main(int argc, char *argv[]) {
    wiringPiSetup();
    pullUpDnControl(0, PUD_DOWN);

    pinMode(2, OUTPUT);

    motors = new Motors(0x32);
    motors->init();
    motors->calculateAndSend(0, 0);

    server = new Server(1337);
    server->bindPort();

    puts("Bound port.");

    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    server->acceptClient(onClient);

    return 0;
}
