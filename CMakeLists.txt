cmake_minimum_required(VERSION 3.10)

project(msconverter)

add_subdirectory(src)

enable_testing()

# file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)

# foreach(testSrc ${TEST_SRCS})
#     get_filename_component(testName ${testSrc} NAME_WE)
#     add_executable(${testName} ${testSrc})
#     target_link_libraries(${testName} ${Boost_LIBRARIES})
#     set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin)
#     add_test(NAME ${testName}
#              WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
#              COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/testBin/${testName})
# endforeach(testSrc)