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
        size_t size = sizeof(int) + sizeof(char);
        char *data = (char *) malloc(size);

        int angle = (int) compass->readAngle();
        *((int *) data) = angle;
        data += sizeof(int);

        char dist = distance_sensor->readDistance();
        *(data) = dist;
        data += sizeof(char);

        data -= size;
        server->writePacket(client, data, size);

        if (!stopping) {
            usleep(100000);
        }
    }
}

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

void onClient(int socket) {
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);
    usleep(100000);
    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    client = socket;

    if (pthread_create(&sensor_thread, NULL, sensorRun, NULL)) {
        cerr << "Error creating sensor thread" << std::endl;
        return;
    }

    server->handlePackets(socket, onPacket, onClose);
}

int main(int argc, char *argv[]) {
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

    server = new Server(1337);
    if (!server->bindPort()) {
        cerr << "Failed to bind port" << std::endl;
        return 1;
    }

    cout << "Port bound" << std::endl;

    digitalWrite(2, 1);
    usleep(5000);
    digitalWrite(2, 0);

    server->acceptClient(onClient);

    stopping = true;

    pthread_join(sensor_thread, NULL);

    return 0;
}
