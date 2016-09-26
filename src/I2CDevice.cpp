#include <wiringPiI2C.h>
#include <unistd.h>
#include "I2CDevice.h"

I2CDevice::I2CDevice(int id) {
    this->id = id;
}

void I2CDevice::init() {
    this->handle = wiringPiI2CSetup(this->id);
}

ssize_t I2CDevice::send(const void *data, size_t size) {
    return write(this->handle, data, size);
}