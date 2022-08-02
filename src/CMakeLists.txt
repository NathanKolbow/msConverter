cmake_minimum_required(VERSION 3.10)

project(newicktoms)

add_subdirectory(core)
add_subdirectory(tests)

# Boost configuration
set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
# set(BOOST_LIBRARYDIR "C:/Program Files (x86)/boost/stage/lib/")
find_package(Boost 1.60.0 COMPONENTS program_options unit_test_framework REQUIRED)
# find_package(Boost 1.79.0)

# Build the SimSuite library
add_library(simsuite OBJECT SimSuite.cpp)
set_property(TARGET simsuite PROPERTY CXX_STANDARD 17)
target_include_directories(simsuite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(simsuite core)

# Set up boost and add the ms-converter executable
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(ms-converter exec.cpp)
    target_link_libraries(ms-converter ${Boost_LIBRARIES} core simsuite)
    set_target_properties(ms-converter PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/)
    set_property(TARGET ms-converter PROPERTY CXX_STANDARD 17)
endif()