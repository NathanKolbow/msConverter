project(core)

add_library(${PROJECT_NAME} OBJECT Network.cpp Node.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
# add_executable(${PROJECT_NAME} core-main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/core
)