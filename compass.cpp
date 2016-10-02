#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define _USE_MATH_DEFINES

int  compass;
void writeToDevice() ;

int main(int argc, char** argv) {
    compass = wiringPiI2CSetup(0x1e);
    writeToDevice();

    while (1) {
        //Configuration
        wiringPiI2CWriteReg8(compass, 0x02, 0x01); //idle mode
        usleep(100000);
        int xm = (char) wiringPiI2CReadReg8(compass, 0x03);
        int xl = (char) wiringPiI2CReadReg8(compass, 0x04);
        int ym = (char) wiringPiI2CReadReg8(compass, 0x07);
        int yl = (char) wiringPiI2CReadReg8(compass, 0x08);
        int zm = (char) wiringPiI2CReadReg8(compass, 0x05);
        int zl = (char) wiringPiI2CReadReg8(compass, 0x06);

//        printf("%d, %d, %d, %d, %d, %d, %d\n", compass, xm, xl, ym, yl, zm, zl);

        // Read from compass
        short x = (xm << 8) | xl;
        short y = (ym << 8) | yl;
        short z = (zm << 8) | zl;

        float angle = (atan2(y, x)) * 180 / M_PI;
        //With just the above formula, we will get the angle between the values -180 to 180.
        //We will use the if statement below to change this to a angle value between 0 and 360.
//        if (angle < 0) {
//            angle += 360;
//        }
        printf("angle=%0.1f,\t x=%d, y=%d, z=%d\n", angle, x, y, z);
    }
}

void writeToDevice() {
    //int compass = wiringPiI2CSetup(0x1e);
    wiringPiI2CWriteReg8(compass, 0x00, 0x70);
    wiringPiI2CWriteReg8(compass, 0x01, 0xA0);
    wiringPiI2CWriteReg8(compass, 0x02, 0x01);

}