#include "uart_sensor.h"
#include <thread>
#include <iostream>

int main() {
    UARTSensor sensor("/dev/ttyUSB0");
    
    if (sensor.open_connection()) {
        sensor.send_command({0x01});
        
        for (int i = 0; i < 5; ++i) {
            auto data = sensor.read_data();
            float value = *reinterpret_cast<float*>(data.data());
            
            std::cout << "Sensor value: " << value << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        sensor.close_connection();
    }

    return 0;
}