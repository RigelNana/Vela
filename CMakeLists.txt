cmake_minimum_required(VERSION 3.30)

project(Orion)

set(CMAKE_CXX_STANDARD 23)
add_executable(test)
target_sources(test
        PUBLIC
        main.cpp
)
target_sources(test
        PUBLIC
        FILE_SET all_my_modules TYPE CXX_MODULES FILES
        units/Angle.cppm
        time/TimeSystem.cppm
        coord/Coord.cppm
        constant/Constant.cppm
)

target_link_libraries(test PRIVATE -lstdc++exp)