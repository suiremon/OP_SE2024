#include <cpr/cpr.h>
#include <ConfigParser.h>
#include <utility>

constexpr u_short kHoursPerDay = 24;
constexpr u_short kTimesOfDay = 4;
constexpr u_short kMetersInKm = 1000;

struct City {
    City(std::string lat, std::string lon, std::string name) : latitude(std::move(lat)), longitude(std::move(lon)),
                                                                   name(std::move(name)) {};
    City() = default;
    ~City() = default;

    std::string latitude;
    std::string longitude;
    std::string name;
};

struct Weather {
    Weather(std::string date, std::string time, const int& temperature, const size_t& visibility,
            std::string weatherCode,
            const int& windSpeed,
            const int& windDirection,
            const u_short& humidity);
    Weather() = default;
    ~Weather() = default;

    std::string m_date;
    std::string m_time;
    int m_temperature{};
    size_t m_visibility{};
    std::string m_weatherCode{};
    unsigned int m_windSpeed{};
    int m_windDirection;
    u_short m_humidity{};
};

class Forecast {
private:
    Config _params;
    City _city;

public:
    explicit Forecast(const std::string&);
    Forecast() = default;

    std::vector<std::vector<Weather>> m_daysForecast;
    [[nodiscard]] std::vector<std::vector<Weather>> getForecast(const u_short&) const;
};
