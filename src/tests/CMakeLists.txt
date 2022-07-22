project(tests)

# Configure and find Boost
set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.60.0 COMPONENTS program_options unit_test_framework REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    # find all test files
    file(GLOB tests "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    foreach(test ${tests})
        get_filename_component(testname ${test} NAME_WE)
        add_executable(${testname} ${CMAKE_CURRENT_SOURCE_DIR}/${testname}.cpp)
        target_link_libraries(${testname} ${Boost_LIBRARIES} core simsuite)
        add_test(NAME ${testname} COMMAND ${testname})
        set_property(TARGET ${testname} PROPERTY CXX_STANDARD 17)
    endforeach()

    # # test_isomorphicNewick
    # add_executable(test_isomorphicNewick ${CMAKE_CURRENT_SOURCE_DIR}/test_isomorphicNewick.cpp)
    # target_link_libraries(test_isomorphicNewick ${Boost_LIBRARIES} core)
    # add_test(NAME test_isomorphicNewick COMMAND test_isomorphicNewick)
    # set_property(TARGET test_isomorphicNewick PROPERTY CXX_STANDARD 17)

    # # test_makeUltrametric
    # add_executable(test_makeUltrametric ${CMAKE_CURRENT_SOURCE_DIR}/test_makeUltrametric.cpp)
    # target_link_libraries(test_makeUltrametric ${Boost_LIBRARIES} core)
    # add_test(NAME test_makeUltrametric COMMAND test_makeUltrametric)
    # set_property(TARGET test_makeUltrametric PROPERTY CXX_STANDARD 17)
endif()