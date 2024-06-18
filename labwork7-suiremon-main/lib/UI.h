#include <WeatherRequest.h>
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

class UI : ftxui::ComponentBase {
private:
    Config _params;
    std::function<void()> _quit;
    int _currentCityId = 0;
    int _currentDayValue = 0;
    int _sliderValue = _params.m_days;
    ftxui::MenuOption _currentCityOption = {};
    ftxui::Component _currentCity = ftxui::Menu({}, &_currentCityId, _currentCityOption);
    std::string _date;
    u_short _hour{};

    void setDate();

public:
    explicit UI();
    ~UI() override = default;
    static void show();
};
