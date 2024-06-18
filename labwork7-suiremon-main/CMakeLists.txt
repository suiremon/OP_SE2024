cmake_minimum_required(VERSION 3.26)
project(labwork7_suiremon)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")
include_directories(bin)
include_directories(lib)
 
add_executable(labwork7_suiremon
        bin/main.cpp
        lib/WeatherRequest.cpp
        lib/WeatherRequest.h
        lib/UI.cpp
        lib/UI.h
        lib/ConfigParser.cpp
        lib/ConfigParser.h)
 
if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
 
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG master) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
 
target_link_libraries(labwork7_suiremon PRIVATE cpr::cpr)
 
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
 
target_link_libraries(labwork7_suiremon PRIVATE nlohmann_json::nlohmann_json)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG main
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(labwork7_suiremon PRIVATE ftxui::component
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom)