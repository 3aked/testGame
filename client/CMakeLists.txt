project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS WebSockets REQUIRED)

set(SRCS main.cpp)
set(HDRS)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${SRCS} ${HDRS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Network
    Qt5::WebSockets
    Qt5::Core
    common)
include_directories(${common_SOURCE_DIR})
