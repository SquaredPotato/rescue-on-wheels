
#include <cstdint>
#include "Distance.h"

Distance::Distance(int id) : I2CDevice(id) {}

void Distance::init() {
    I2CDevice::init();

    this->startMeasurement();
}

void Distance::startMeasurement() {
    this->writeReg8(0, 0x51);
}

uint8_t Distance::readDistance() {
    uint8_t dist = (uint8_t) this->readReg8(0x03);

    this->startMeasurement();

    return dist;
}
