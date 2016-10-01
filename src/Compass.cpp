
#include <zconf.h>
#include <cmath>
#include "Compass.h"

Compass::Compass(int id) : I2CDevice(id) {}

void Compass::init() {
    I2CDevice::init();

    this->writeReg8(0x00, 0x70);
    this->writeReg8(0x01, 0xA0);
    this->writeReg8(0x02, 0x01);
}

double Compass::readAngle() {
    this->writeReg8(0x02, 0x01); //idle mode
    usleep(100000);
    int xm = this->readReg8(0x03);
    int xl = this->readReg8(0x04);
    int ym = this->readReg8(0x07);
    int yl = this->readReg8(0x08);

    // Read from compass
    int x = (xm << 8) | xl;
    int y = (ym << 8) | yl;

    double angle = (atan2(y, x)) * 180 / M_PI;
    //With just the above formula, we will get the angle between the values -180 to 180.
    //We will use the if statement below to change this to a angle value between 0 and 360.
    if (angle < 0) {
        angle += 360;
    }

    return angle;
}
