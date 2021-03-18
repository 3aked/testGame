project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS WebSockets REQUIRED)

set(SOURCES main.cpp game.cpp server.cpp)
set(HEADERS server.h game.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    Qt5::Network
    Qt5::WebSockets
    Qt5::Core
    common)

include_directories(${common_SOURCE_DIR})
