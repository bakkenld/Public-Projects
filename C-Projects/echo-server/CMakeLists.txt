cmake_minimum_required(VERSION 3.10)
project(PriorityEchoServer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags for safety and debugging
add_compile_options(-Wall -Wextra -Wformat -Wpedantic -g -pthread)

if(NOT DEFINED TARGET_LOG_LEVEL)
    set(TARGET_LOG_LEVEL 0) # Default logging level is none
endif()

add_definitions(-DTARGET_LOG_LEVEL=${TARGET_LOG_LEVEL})

# Include Directories
include_directories(
    include 
    lib/logger 
    lib/tpool
    lib/dispatch
    lib/startup
)

# Find Dependencies
find_package(Threads REQUIRED)

# Check for CMocka for testing
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(CMOCKA cmocka)
endif()

# Create the Core Utility Library
# Note the updated paths to match your /lib and /src structure
add_library(server_core STATIC 
    src/eserver.c 
    lib/logger/logger.c 
    lib/tpool/tpool.c
    lib/dispatch/dispatch.c
    lib/startup/startup.c
)
target_link_libraries(server_core PRIVATE Threads::Threads)

# Main Executable
add_executable(echo_server src/main.c)
target_link_libraries(echo_server PRIVATE server_core Threads::Threads)

# Unit Testing
if(CMOCKA_FOUND)
    enable_testing()
    # TPOOL TESTS 
    add_executable(test_tpool test/test_tpool.c)
    target_link_libraries(test_tpool PRIVATE server_core Threads::Threads ${CMOCKA_LIBRARIES})
    add_test(NAME TestThreadPool COMMAND test_tpool)

    # DISPATCH TESTS 
    add_executable(test_dispatch test/test_dispatch.c)
    target_link_libraries(test_dispatch PRIVATE server_core Threads::Threads ${CMOCKA_LIBRARIES})
    add_test(NAME TestDispatch COMMAND test_dispatch)
else()
    message(STATUS "CMocka not found. Unit tests will not be built.")
endif()