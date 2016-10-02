#include <wiringPi.h>
#include <pthread.h>
#include <iostream>
#include "Server.h"
#include "Motors.h"
#include "Compass.h"
#include "Distance.h"

using namespace std;

pthread_t sensor_thread;

Compass *compass;
Distance *distance_sensor;
Motors *motors;
Server *server;

int client;

bool stopping = false;

void *sensorRun(void *vd) {
    while (!stopping) {
        size_t size = sizeof(int) * 2;
        char *data = (char *) malloc(size);

        int angle = (int) compass->readAngle();
        *((int *) data) = angle;
        data += sizeof(int);

        int dist = distance_sensor->readDistance();
        *((int *) data) = dist;
        data += sizeof(int);

        data -= size;
        server->writePacket(client, data, size);

        if (!stopping) {
            usleep(100000);
        }
    }
}

void bleep() {
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);
}

void onClose() {
    bleep();

    motors->calculateAndSend(0, 0);
}

void onPacket(char *data, size_t size) {
    int16_t angle = *((int16_t *) data);
    char speed = data[2];
    char buzz  = data[3];

    motors->calculateAndSend(angle, sqrt(speed) * 100);

    digitalWrite(2, buzz);
}

void onClient(int socket) {
    bleep();
    usleep(100000);
    bleep();

    client = socket;

    if (pthread_create(&sensor_thread, NULL, sensorRun, NULL)) {
        cerr << "Error creating sensor thread" << std::endl;
        return;
    }

    server->handlePackets(socket, onPacket, onClose);
}

void setupHardware() {
    wiringPiSetup();
    pullUpDnControl(0, PUD_DOWN);

    pinMode(2, OUTPUT);

    motors = new Motors(0x32);
    motors->init();
    motors->calculateAndSend(0, 0);

    compass = new Compass(0x1e);
    compass->init();

    distance_sensor = new Distance(0x70);
    distance_sensor->init();
}

void setupServer() {
    server = new Server(1337);
    if (!server->bindPort()) {
        cerr << "Failed to bind port" << std::endl;
        return;
    }

    cout << "Port bound" << std::endl;
}

int main(int argc, char *argv[]) {

    setupHardware();
    setupServer();

    bleep();

    server->acceptClient(onClient);

    stopping = true;

    pthread_join(sensor_thread, NULL);

    return 0;
}
