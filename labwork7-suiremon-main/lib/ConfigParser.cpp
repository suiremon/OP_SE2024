#include "ConfigParser.h"


Config::Config() {
    std::ifstream f("config.json");
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    if (data["cities"] != nullptr && data["frequency"] != nullptr && data["frequency"] > 0 && data["days"] != nullptr &&
        data["days"] > 0 && data["days"] < 5 && data["API key"] != nullptr) {
        m_apiKey = data["API key"];
        for (const auto& city : data["cities"]) {
            m_cityNames.push_back(city);
        }
        m_freq = data["frequency"];
        m_days = data["days"];
    }
    else {
        std::cerr << "Incorrect config.";
        exit(EXIT_FAILURE);
    }
}
