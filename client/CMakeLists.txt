project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS WebSockets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SRCS main.cpp client.cpp game.cpp mainwindow.cpp clientdefwidget.cpp ingamewidget.cpp finish.cpp)
set(HDRS client.h game.h mainwindow.h clientdefwidget.h ingamewidget.h finish.h)
set(FORMS mainwindow.ui clientdefwidget.ui ingamewidget.ui finish.ui)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${SRCS} ${HDRS} ${FORMS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Network
    Qt5::WebSockets
    Qt5::Core
    Qt5::Widgets
    common)

include_directories(${common_SOURCE_DIR})
