#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

    bool isWritable(long address) {
        return read(address) == WRITABLE;
    }
    const int WRITABLE = 0xFF;
protected:
    FlashMemoryDevice* m_hardware;
};