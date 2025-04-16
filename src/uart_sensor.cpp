#include "uart_sensor.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

UARTSensor::UARTSensor(const std::string& port, float mean, float stddev)
    : port_name(port),
      generator(std::chrono::system_clock::now().time_since_epoch().count()),
      data_distribution(mean, stddev) {
    std::cout << "UARTSensor initialized on port: " << port_name << "\n";
    generate_sensor_data();
}

UARTSensor::~UARTSensor() {
    std::cout << "UARTSensor destroyed for port: " << port_name << "\n";
}

bool UARTSensor::open_connection() {
    std::cout << "[UART] Connection opened on " << port_name << "\n";
    return true;
}

void UARTSensor::close_connection() {
    std::cout << "[UART] Connection closed on " << port_name << "\n";
}

void UARTSensor::generate_sensor_data() {
    current_value = data_distribution(generator);
    std::max(0.0f, std::min(100.0f, current_value));
}

std::vector<uint8_t> UARTSensor::read_data() {
    generate_sensor_data();
    auto value_ptr = reinterpret_cast<uint8_t*>(&current_value);
    return std::vector<uint8_t>(value_ptr, value_ptr + sizeof(float));
}

void UARTSensor::send_command(const std::vector<uint8_t>& command) {
    std::cout << "[UART] Command sent: ";
    for (auto byte : command) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "\n";

    if (!command.empty() && command[0] == 0x01) {
        std::cout << "[UART] Reset command received. Reinitializing sensor...\n";
        generate_sensor_data();
    }
}
