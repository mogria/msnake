cmake_minimum_required(VERSION 2.8.4)
project(msnake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(SNAKE_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(SNAKE_TEST_PATH ${PROJECT_SOURCE_DIR}/tests)

# we need ncurses as a dependency
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)
include_directories(${CURSES_INCLUDE_DIRS})

# game executable
file(GLOB_RECURSE SNAKE_SOURCE_FILES ${SNAKE_SOURCE_PATH}/*.c)
add_executable(msnake ${SNAKE_SOURCE_FILES})
target_link_libraries(msnake ${CURSES_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(msnake rt)
endif()

install(TARGETS msnake DESTINATION bin)

# test runner executable
find_package(Check)
execute_process(
    COMMAND ${PROJECT_SOURCE_DIR}/bin/prepare-tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
    RESULT_VARIABLE prep_test_result)
add_executable(msnake_tests ${SNAKE_TEST_PATH}/main.c src/fruits.c src/effects.c)
target_link_libraries(msnake_tests ${CHECK_LIBRARIES} ${CURSES_LIBRARIES})
add_test(NAME msnake_tests COMMAND msnake_tests)
include(CTest)

