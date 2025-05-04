cmake_minimum_required(VERSION 3.28)
project(STP)

set(CMAKE_CXX_STANDARD 17)

# Поиск Sofia-SIP через pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SofiaSIP REQUIRED sofia-sip-ua)

include_directories("header")

file(GLOB_RECURSE SOURCE
        "source/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${SofiaSIP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SofiaSIP_LIBRARIES})