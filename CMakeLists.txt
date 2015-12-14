cmake_minimum_required(VERSION 2.8.4)
project(msnake)

set(CMAKE_C_FLAGS "-Wall")

set(CURSES_NEED_NCURSES TRUE)
find_package(curses)

include_directories(${CURSES_INCLUDE_DIRS})

set(SNAKE_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SNAKE_SOURCE_FILES ${SNAKE_SOURCE_PATH}/*.c)
add_executable(msnake ${SNAKE_SOURCE_FILES})
target_link_libraries(msnake ${CURSES_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${SNAKE_EXEC_NAME} -lrt)
endif()

install(TARGETS msnake DESTINATION /usr/local/bin)
