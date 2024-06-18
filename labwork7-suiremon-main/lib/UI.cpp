#include "UI.h"


void UI::setDate() {
    const std::time_t t = std::time(nullptr);
    const std::tm* now = std::localtime(&t);
    _date = "Current date: " + (now->tm_mday < 10 ? "0" + std::to_string(now->tm_mday) : std::to_string(now->tm_mday))
        + '.' + (now->tm_mon < 9 ? "0" + std::to_string(now->tm_mon + 1) : std::to_string(now->tm_mon + 1))
        + '.' + std::to_string(now->tm_year + 1900);
    _hour = now->tm_hour;
}

UI::UI() {
    setDate();

    Add(_currentCity);
    for (const auto& city : _params.m_cityNames) {
        _currentCity->Add(ftxui::MenuEntry(city));
    }
}

void renderKeys(const ftxui::Event& event, const std::vector<std::string>& keys, int& val, const int& delta) {
    for (auto& i : keys) {
        if (event.input() == i) {
            val += delta;
            return;
        }
    }
}

void keyEvent(const ftxui::Event& event, int& currentCityId, int& sliderValue) {
    renderKeys(event, {"n", "N", "т", "T"}, currentCityId, 1);
    renderKeys(event, {"p", "P", "з", "З"}, currentCityId, -1);
    renderKeys(event, {"+", "="}, sliderValue, 1);
    renderKeys(event, {"-", "_"}, sliderValue, -1);
}


ftxui::Element getPicture(const std::string& _weatherCode) {
    std::ifstream f("weather.json");
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    std::vector<ftxui::Element> picture;
    if (data[_weatherCode] != nullptr) {
        for (size_t i = 0; i < data[_weatherCode].size(); ++i) {
            std::string tmp = data[_weatherCode][i];
            picture.push_back(ftxui::text(tmp));
        }
    }
    return vbox(picture) | ftxui::center | size(ftxui::WIDTH, ftxui::GREATER_THAN, 21);
}

std::string getDirectionMark(const int& windDirection) {
    const std::vector<std::string> marks = {"🡭 ", "🡪 ", "🡮 ", "🡫 ", "🡯 ", "🡨 ", "🡬 ", "🡩 "};
    int angles[] = {15, 75, 105, 165, 195, 255, 285, 345, 360, 0};

    for (int i = 0; i < marks.size(); i++) {
        if (windDirection > angles[i] && windDirection <= angles[i + 1]) {
            return marks[i];
        }
    }
    return marks[marks.size()];
}

ftxui::Element getBox(const Weather& dayForecast) {
    auto pic = getPicture(dayForecast.m_weatherCode);

    auto timeBox = hbox(ftxui::text(dayForecast.m_time)) | ftxui::center;
    auto values = vbox(ftxui::text(std::to_string(dayForecast.m_temperature) + "°C"),
                       ftxui::text(std::to_string(dayForecast.m_humidity) + "%"),
                       ftxui::text(std::to_string(dayForecast.m_visibility) + " km"),
                       ftxui::text(getDirectionMark(dayForecast.m_windDirection) +
                           std::to_string(dayForecast.m_windSpeed) + " km/h"));

    return window(timeBox, hbox(pic, values | ftxui::center) | ftxui::flex) | ftxui::flex;
}

ftxui::Component getForecast(const std::vector<Weather>& forecast) {

    auto morning = ftxui::Renderer([&] { return getBox(forecast[0]); });
    auto day = ftxui::Renderer([&] { return getBox(forecast[1]); });
    auto evening = ftxui::Renderer([&] { return getBox(forecast[2]); });
    auto night = ftxui::Renderer([&] { return getBox(forecast[3]); });

    auto timestampContainer = ftxui::Container::Horizontal({ftxui::Renderer([&] { return ftxui::text(forecast[0].m_date); })});
    auto forecastContainer = ftxui::Container::Horizontal({morning, day, evening, night}) | ftxui::flex;

    auto window = ftxui::Container::Vertical({timestampContainer | ftxui::center, forecastContainer}) | ftxui::flex;
    return window;
}

void UI::show() {
    UI ui;
    std::unordered_map<std::string, Forecast> forecasts;

    for (const auto& i : ui._params.m_cityNames) {
        forecasts[i] = Forecast(i);
    }
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    auto slider = ftxui::Slider("", &ui._sliderValue, 0, 4, 1);
    auto cityMenu = ftxui::Menu(&ui._params.m_cityNames, &ui._currentCityId);

    const auto menuComponents = ftxui::Container::Vertical({slider, cityMenu});
    ftxui::Component forecastContainer = ftxui::Container::Horizontal({ftxui::Renderer([&] { return ftxui::hbox({}); })});

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    now->tm_hour += ui._params.m_freq;

    const auto renderer = Renderer(menuComponents, [&]{
        if (menuComponents->ActiveChild() == cityMenu) {
            menuComponents->SetActiveChild(slider);
        }
        forecastContainer = ftxui::Container::Vertical({ftxui::Renderer([&] { return ftxui::hbox({}); })});
        for (int i = 0; i < ui._sliderValue; ++i) {
            forecastContainer->Add(getForecast(forecasts[ui._params.m_cityNames[ui._currentCityId]].m_daysForecast[i]));
        }
        if (ui._hour >= now->tm_hour) {
            for (const auto& i : ui._params.m_cityNames) {
                forecasts[i] = Forecast(i);
            }
            t = std::time(nullptr);
            now = std::localtime(&t);
            ui._hour = now->tm_hour;
            now->tm_hour += ui._params.m_freq;
        }
        auto sideBox = ftxui::vbox({
            ftxui::text("Weather Forecast") | ftxui::center,
            ftxui::separator(),
            center(ftxui::text("City")),
            cityMenu->Render() |ftxui:: flex,
            ftxui::separator(),
            center(ftxui::text("Days")),
            slider->Render()
        });
        auto mainBox = ftxui::vbox({
            center(ftxui::text(ui._date)),
            ftxui::separator(),
            forecastContainer->Render() | ftxui::flex
        }) | ftxui::flex;

        return borderHeavy(ftxui::hbox({sideBox, ftxui::separator(), mainBox}) | ftxui::flex);
    });

    auto keybindHelp = ftxui::Renderer([&] {
        return ftxui::vbox({
            center(ftxui::text("[n]/[p] to switch between cities")),
            center(ftxui::text("[+]/[-] to change the number of forecast days"))
        });
    });

    auto keybinding = CatchEvent(renderer, [&](const ftxui::Event& event) {
        if (event == ftxui::Event::Escape) {
            screen.ExitLoopClosure()();
        }
        else {
            keyEvent(event, ui._currentCityId, ui._sliderValue);
        }
        return false;
    });

    const auto forecasterInterface = ftxui::Container::Vertical({keybinding, keybindHelp}) | ftxui::flex;
    screen.Loop(forecasterInterface);
}
