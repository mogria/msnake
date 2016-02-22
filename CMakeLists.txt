cmake_minimum_required(VERSION 2.8.4)
project(msnake)

set(CMAKE_C_FLAGS "-Wall")

include(CheckLibraryExists)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)

include_directories(${CURSES_INCLUDE_PATH})

set(SNAKE_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SNAKE_SOURCE_FILES ${SNAKE_SOURCE_PATH}/*.c)
add_executable(msnake ${SNAKE_SOURCE_FILES})
target_link_libraries(msnake ${CURSES_LIBRARIES})

if(MINGW)
    target_link_libraries(msnake pthread)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(msnake rt)
endif()

install(TARGETS msnake DESTINATION /usr/local/bin)
