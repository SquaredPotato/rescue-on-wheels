
#ifndef COLUMN_MOTORS_H
#define COLUMN_MOTORS_H

#include "I2CDevice.h"

class Motors : public I2CDevice {
public:
    Motors(int id);
    void sendSpeed();
    void calculateSpeed(int rotation, double speed);
    void calculateAndSend(int rotation, double speed);

    virtual void init() override;

private:
    double leftSpeed;
    double rightSpeed;

    bool leftDir;
    bool rightDir;
};


#endif //COLUMN_MOTORS_H
