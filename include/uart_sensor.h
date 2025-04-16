#pragma once

#include <vector>
#include <string>
#include <random>
#include <fstream>

class UARTSensor {
private:
    std::string port_name;           
    std::mt19937 generator;          
    std::normal_distribution<float> data_distribution; 
    float current_value;        

    void generate_sensor_data();
    void log(const std::string& message);
    
public:
    UARTSensor(const std::string& port, float mean = 50.0f, float stddev = 10.0f);
    ~UARTSensor();
    bool open_connection();
    void close_connection();
    std::vector<uint8_t> read_data();
    void send_command(const std::vector<uint8_t>& command);
};

