
#ifndef COLUMN_COMPASS_H
#define COLUMN_COMPASS_H


#include "I2CDevice.h"
#include <math.h>

class Compass : public I2CDevice {
public:
    Compass(int id);

    virtual void init() override;

    float readAngle();
};


#endif //COLUMN_COMPASS_H
