project(core)

add_library(${PROJECT_NAME} OBJECT Network.cpp Node.cpp)
# add_executable(${PROJECT_NAME} core-main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/core
)