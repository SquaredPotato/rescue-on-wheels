
#ifndef COLUMN_DISTANCE_H
#define COLUMN_DISTANCE_H


#include <cstdint>
#include "I2CDevice.h"

class Distance : public I2CDevice {
public:
    Distance(int id);

    virtual void init() override;

    void startMeasurement();
    uint8_t readDistance();
};


#endif //COLUMN_DISTANCE_H
