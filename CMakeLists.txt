cmake_minimum_required(VERSION 3.16)
project(fball)

set(CMAKE_CXX_STANDARD 11)
FILE(GLOB TARGET_H "${CMAKE_SOURCE_DIR}/*.h" "motor_driver/*.h")
FILE(GLOB TARGET_CPP "${CMAKE_SOURCE_DIR}/*.cpp" "motor_driver/*.cpp")

SET(TARGET_SRC ${TARGET_H} ${TARGET_CPP} )

ADD_LIBRARY(LibsModule
        ${TARGET_SRC}
        )

target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule -lrt)
target_link_libraries(LibsModule -lpigpiod_if2)

add_executable(fball ${TARGET_SRC})
target_link_libraries(fball LibsModule)