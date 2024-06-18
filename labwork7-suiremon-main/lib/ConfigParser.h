#include <vector>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

class Config {
public:
    Config();
    ~Config() = default;

    std::vector<std::string> m_cityNames;
    uint16_t m_freq;
    int m_days;
    std::string m_apiKey;
};