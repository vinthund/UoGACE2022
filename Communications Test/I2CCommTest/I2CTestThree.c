#include <iostream>
#include <wiringPiI2C.h>
#define DEVICE_ID 0x08
int main (int argc, char **argv)
{
    // Setup I2C communication
    int fd = wiringPiI2CSetup(DEVICE_ID);
    if (fd == -1) {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    }
    std::cout << "I2C communication successfully setup.\n";
    // Send data to arduino
    uint8_t data_to_send = 17;
    wiringPiI2CWrite(fd, data_to_send);
    std::cout << "Sent data: " << (int)data_to_send << "\n";
    // Read data from arduino
    int received_data = wiringPiI2CRead(fd);
    std::cout << "Data received: " << received_data << "\n";
    if (received_data == data_to_send) {
        std::cout << "Success!\n";
    }
    return 0;
}
