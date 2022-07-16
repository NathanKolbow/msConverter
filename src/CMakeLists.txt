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

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(myexec exec.cpp)
    target_link_libraries(myexec ${Boost_LIBRARIES})
endif()

set(tool_dest "bin")
install(TARGETS myexec DESTINATION "bin")