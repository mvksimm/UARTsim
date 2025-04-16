#include "uart_sensor.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime> 
#include <iomanip>

UARTSensor::UARTSensor(const std::string& port, float mean, float stddev)
    : port_name(port),
      generator(std::chrono::system_clock::now().time_since_epoch().count()),
      data_distribution(mean, stddev) {

    log_file.open("uart_log.txt", std::ios::app);
    log("UARTSensor initialized on port: " + port);
    
    generate_sensor_data();
}

UARTSensor::~UARTSensor() {
    log("UARTSensor destroyed for port: " + port_name);
    if (log_file.is_open()) {
        log_file.close();
    }
}

void UARTSensor::log(const std::string& message) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string timestamp = std::string(std::ctime(&now));
    timestamp.pop_back(); // удалить \n

    std::string full_message = "[" + timestamp + "] " + message;

    std::cout << full_message << std::endl;
    if (log_file.is_open()) {
        log_file << full_message << std::endl;
    }
}

bool UARTSensor::open_connection() {
    log("Connection opened on " + port_name);
    return true;
}

void UARTSensor::close_connection() {
    log("Connection closed on " + port_name);
}

void UARTSensor::generate_sensor_data() {
    current_value = data_distribution(generator);
    std::max(0.0f, std::min(100.0f, current_value));
}

std::vector<uint8_t> UARTSensor::read_data() {
    generate_sensor_data();
    log("Sensor value generated: " + std::to_string(current_value));

    auto value_ptr = reinterpret_cast<uint8_t*>(&current_value);
    return std::vector<uint8_t>(value_ptr, value_ptr + sizeof(float));
}

void UARTSensor::send_command(const std::vector<uint8_t>& command) {
    std::ostringstream oss;
    oss << "Command sent: ";
    for (auto byte : command) {
        oss << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    log(oss.str());

    if (!command.empty() && command[0] == 0x01) {
        log("Reset command received.");
        generate_sensor_data();
    }
}
