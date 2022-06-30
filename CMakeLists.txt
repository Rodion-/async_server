cmake_minimum_required(VERSION 3.16)

project(async_server VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -pthread")


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})


IF (NOT DEFINED TEST_RUN)
    SET(TEST_RUN 0 )
ENDIF()

ADD_DEFINITIONS(-DTEST_RUN=${TEST_RUN})

add_executable(async_server main.cpp async_server.cpp command.cpp )

target_include_directories(async_server PUBLIC
                           "${PROJECT_BINARY_DIR}"
			    "${GTEST_INCLUDE_DIRS}"
                           )
                                        
target_link_libraries(async_server PUBLIC "${GTEST_BOTH_LIBRARIES}"  )                                                     
                           	   
install(TARGETS async_server DESTINATION bin)
