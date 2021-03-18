project(common)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(${PROJECT_NAME}  STATIC)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(SRCS
    errors.cpp
    data.cpp)

set(HDRS
    errors.h
    data.h)

target_sources(${PROJECT_NAME} PRIVATE ${SRCS} ${HDRS})
target_link_libraries(${PROJECT_NAME} Qt5::Core)
