
#ifndef COLUMN_I2CDEVICE_H
#define COLUMN_I2CDEVICE_H


#include <cstddef>

class I2CDevice {
public:
    I2CDevice(int id);

    virtual void init();
    int send(const void *data, size_t size);
    int writeReg8(int reg, int data);
    int readReg8(int reg);

private:
    int id;
protected:
    int handle;
};


#endif //COLUMN_I2CDEVICE_H
