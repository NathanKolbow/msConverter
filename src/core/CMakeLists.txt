project(core)

add_library(core OBJECT Network.cpp Node.cpp)
set_property(TARGET core PROPERTY CXX_STANDARD 17)

set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.60.0 REQUIRED COMPONENTS program_options unit_test_framework)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/core
)