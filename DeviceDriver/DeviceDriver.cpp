#include "DeviceDriver.h"
#include <iostream>

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : 
    m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int temp = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++) {
        if (temp != (int)(m_hardware->read(address)))
            throw std::runtime_error("Five read values should be same.");
    }
    return temp;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    if(isWritable(address) == false)
        throw std::runtime_error("The address is not empty.");
    m_hardware->write(address, (unsigned char)data);
}