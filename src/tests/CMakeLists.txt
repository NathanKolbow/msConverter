project(tests)

add_library(${PROJECT_NAME} OBJECT CoreTests.cpp)

# Configure and find Boost
set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.70.0 COMPONENTS program_options unit_test_framework REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(test_runner ${CMAKE_CURRENT_SOURCE_DIR}/CoreTests.cpp)
    target_link_libraries(test_runner ${Boost_LIBRARIES} core)
    add_test(NAME test_runner COMMAND test_runner)
endif()