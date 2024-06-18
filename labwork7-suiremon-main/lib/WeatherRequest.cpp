#include "WeatherRequest.h"

Weather::Weather(std::string date, std::string time, const int& temperature, const size_t& visibility,
                 std::string weatherCode, const int& windSpeed,const int& windDirection, const u_short& humidity) {
    m_date = std::move(date);
    m_time = std::move(time);
    m_temperature = temperature;
    m_weatherCode = std::move(weatherCode);
    m_windSpeed = windSpeed;
    m_windDirection = windDirection;
    m_humidity = humidity;
    m_visibility = visibility / kMetersInKm;
}

std::string getCoord(const std::string& json, const std::string& name) {
    std::string coord;
    size_t start = json.find(name);
    size_t end = json.find(',', start);
    for (size_t i = start; i < end; ++i) {
        if (atoi(&json[i]) != 0) {
            start = i;
            break;
        }
    };
    for (size_t i = start + 1; i < end; ++i) {
        coord += json[i];
    }
    return coord;
}

std::string MakeCoordURL(const std::string& name) {
    std::string result = name;
    if (name.find(' ') != std::string::npos) {
        result.replace(name.find(' '), 1, "_");
    }
    result = "https://api.api-ninjas.com/v1/city?name=" + result;
    return result;
}

Forecast::Forecast(const std::string& name) {
    cpr::Response r = Get(cpr::Url{MakeCoordURL(name)}, cpr::Header{{"X-Api-Key", _params.m_apiKey}});

    if (r.status_code == 200 && r.text != "[]") {
        _city.name = name;
        _city.latitude = getCoord(r.text, "latitude");
        _city.longitude = getCoord(r.text, "longitude");
        m_daysForecast = getForecast(_params.m_days);
    } else {
        std::cerr << "Invalid request.";
        exit(EXIT_FAILURE);
    }
}

std::string MakeForecastURL(const std::string& lat, const std::string& lon, const u_short& days) {
    std::string url = "https://api.open-meteo.com/v1/forecast?";
    url += "latitude=" + lat;
    url += "&longitude=" + lon;
    url += "&hourly=temperature_2m,visibility,weather_code,wind_speed_10m,wind_direction_10m,relative_humidity_2m";
    url += "&timezone=Europe%2FMoscow&time_mode=time_interval";
    url += "&forecast_days=" + std::to_string(days);
    return url;
}

std::string formatDate(const std::string& date) {
    std::string result;
    std::string tmp;
    for (int i = date.size() - 1; i >= 0; --i) {
        if (date[i] == '-') {
            for (int j = tmp.size() - 1; j >= 0; --j) {
                result += tmp[j];
            }
            tmp = "";
            result += '.';
        } else {
            tmp += date[i];
        }
    }
    for (int j = tmp.size() - 1; j >= 0; --j) {
        result += tmp[j];
    }
    return result;
}

std::vector<std::vector<Weather>> Forecast::getForecast(const u_short& days = -1) const {
    std::vector<std::vector<Weather>> forecast;
    cpr::Response r = Get(cpr::Url{MakeForecastURL(_city.latitude, _city.longitude, 5)});

    if (r.status_code == 200) {
        nlohmann::json info = nlohmann::json::parse(r.text)["hourly"];
        for (size_t j = 0; j < info["time"].size() / kHoursPerDay; ++j) {
            std::vector<Weather> temp;
            for (size_t i = j * kHoursPerDay; i < (j + 1) * kHoursPerDay; i += kHoursPerDay / kTimesOfDay) {
                std::string time = info["time"][i];
                int weatherCode = info["weather_code"][i];

                Weather current(formatDate(time.substr(0, time.find('T'))), time.substr(time.find('T') + 1),
                                info["temperature_2m"][i], info["visibility"][i],
                                std::to_string(weatherCode), info["wind_speed_10m"][i],
                                info["wind_direction_10m"][i], info["relative_humidity_2m"][i]);
                temp.push_back(current);
            }
            forecast.push_back(temp);
        }
    } else {
        std::cout << "Invalid request.";
        exit(EXIT_FAILURE);
    }
    return forecast;
}
