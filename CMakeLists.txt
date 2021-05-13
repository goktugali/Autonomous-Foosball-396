cmake_minimum_required(VERSION 3.16)
project(fball)

set(CMAKE_CXX_STANDARD 11)
FILE(GLOB TARGET_H "${CMAKE_SOURCE_DIR}/*.hpp" "motor_driver/*.hpp" "ball_tracker/*.hpp" "system_control/*.hpp" "game_control/*.hpp")
FILE(GLOB TARGET_CPP "${CMAKE_SOURCE_DIR}/*.cpp" "motor_driver/*.cpp" "ball_tracker/*.cpp" "system_control/*.cpp" "game_control/*.cpp")

SET(TARGET_SRC ${TARGET_H} ${TARGET_CPP} )

ADD_LIBRARY(LibsModule
        ${TARGET_SRC}
        )

target_link_libraries(LibsModule -lpthread)
target_link_libraries(LibsModule -pthread)
target_link_libraries(LibsModule -lrt)
target_link_libraries(LibsModule -lpigpiod_if2)
target_link_libraries(LibsModule -lopencv_core)
target_link_libraries(LibsModule -lopencv_highgui)
target_link_libraries(LibsModule -lopencv_imgproc)
target_link_libraries(LibsModule -lopencv_videoio)
target_link_libraries(LibsModule -lopencv_video)

add_executable(fball ${TARGET_SRC})
target_link_libraries(fball LibsModule)