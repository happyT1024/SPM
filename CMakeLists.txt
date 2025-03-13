cmake_minimum_required(VERSION 3.28)
project(STP)

set(CMAKE_CXX_STANDARD 17)

include_directories("header")

file(GLOB_RECURSE SOURCE
        "source/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE})