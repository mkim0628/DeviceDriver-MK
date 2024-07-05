#include "DeviceDriver.h"
#include <iostream>
using namespace std;

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : 
    m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int temp = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; i++) {
        if (temp != (int)(m_hardware->read(address)))
            throw std::exception("ReadFailException");
    }
    return temp;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    if(isWritable(address) == false)
        throw std::exception("WriteFailException");
    m_hardware->write(address, (unsigned char)data);
}

class App {
public:
    App(DeviceDriver deviceDriver) : deviceDriver(deviceDriver) {};

    void ReadAndPrint(int startAddr, int endAddr) {
        for (int addr = startAddr; addr <= endAddr; addr++) {
            cout << deviceDriver.read(addr) << " ";
        }
        cout << endl;
    }

    void WriteAll(int value) {
        deviceDriver.write(0x00, value);
        deviceDriver.write(0x01, value);
        deviceDriver.write(0x02, value);
        deviceDriver.write(0x03, value);
        deviceDriver.write(0x04, value);
    }

    DeviceDriver deviceDriver;
};