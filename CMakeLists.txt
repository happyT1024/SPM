cmake_minimum_required(VERSION 3.28)
project(STP)

set(CMAKE_CXX_STANDARD 17)
set(TESTS ON)

# Поиск Sofia-SIP через pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SofiaSIP REQUIRED sofia-sip-ua)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.74.0 REQUIRED COMPONENTS system thread regex log)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_LOG_LIBRARY})

include_directories("header")

file(GLOB_RECURSE SOURCE
        "source/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${SofiaSIP_INCLUDE_DIRS} )
target_link_libraries(${PROJECT_NAME} PRIVATE ${SofiaSIP_LIBRARIES} -lboost_log_setup ${USED_LIBS} )


if(TESTS)
    file(GLOB_RECURSE TEST_SOURCE
            "tests/*.cpp"
    )
    enable_testing()
    find_package(GTest REQUIRED)
    add_library(${PROJECT_NAME}_lib ${SOURCE})
    include_directories("tests")
    target_link_libraries(${PROJECT_NAME}_lib -lboost_log_setup ${USED_LIBS})
    add_executable(${PROJECT_NAME}_tests ${TEST_SOURCE})
    target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib -lgtest -lpthread -lgmock)
endif()