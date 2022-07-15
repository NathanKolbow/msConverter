project(tests)

add_library(${PROJECT_NAME} OBJECT CoreTests.cpp)
find_package(Boost 1.79.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})