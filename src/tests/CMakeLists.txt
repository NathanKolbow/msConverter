project(tests)

add_library(${PROJECT_NAME} OBJECT CoreTests.cpp)

set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
# set(BOOST_LIBRARYDIR "C:/Program Files (x86)/boost/stage/lib/")
find_package(Boost 1.70.0 COMPONENTS program_options unit_test_framework REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(test_runner ${CMAKE_CURRENT_SOURCE_DIR}/CoreTests.cpp)
    target_link_libraries(test_runner ${Boost_LIBRARIES} core)
    add_test(NAME test_runner COMMAND test_runner)
endif()
#     get_filename_component(testName ${testSrc} NAME_WE)
#     add_executable(${testName} ${testSrc})
#     target_link_libraries(${testName} ${Boost_LIBRARIES})
#     set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
#     add_test(NAME ${testName}
#              WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
#              COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName})