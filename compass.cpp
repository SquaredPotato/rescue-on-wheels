//
// Created by Stefan Schokker on 19/09/2016.
//
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <cinttypes>

int cd;

int main (void)
{
    wiringPiSetup();
    cd = wiringPiI2CSetup(0x1E);

    //pointer addresses
    uint8_t modeRegisterAddress[1] = {0x02};
    uint8_t outputAddress[1] = {0x03};
    uint8_t writeP[1] = {0x3C};
    uint8_t readP[1] = {0x3D};
    uint8_t modeRegister[1] = {0};

    //setting to continuous mode
    write(cd, writeP, 2);
    write(cd, outputAddress, 4);
    read(cd, &modeRegister, 1);
    modeRegister[0] |= 1 << 8;
    write(cd, writeP, 2);
    write(cd, outputAddress, 4);
    write(cd, modeRegister, 1);

    printf("%s %" PRIu8 " \n", "mode: ", modeRegister);

    while (1 && !getchar())
    {

        //data from output reg
        uint8_t bytes[6] = {0,0,0,0,0,0};

        //move pionter location to Data Output X MSB Register
        write(cd, writeP, 2);
        write(cd, outputAddress, 2);

        for (int i = 0; i < 6; i ++)
        {
            read(cd, &bytes[i], 1);
        }

        int xLocation = (bytes[0] << 8) + (bytes[1] >> 8);
        int yLocation = (bytes[2] << 8) + (bytes[3] >> 8);
        int zLocation = (bytes[4] << 8) + (bytes[5] >> 8);

        printf("%s %d \n", "xLoc: ", xLocation);
        printf("%s %d \n", "yLoc: ", yLocation);
        printf("%s %d \n", "zLoc: ", zLocation);
        printf("%s \n", "----------------");

        usleep(1000000);
    }
}
