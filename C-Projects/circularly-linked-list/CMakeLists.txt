cmake_minimum_required(VERSION 3.10)
project(CircLinkedList C)

add_definitions(-DTARGET_LOG_LEVEL=${TARGET_LOG_LEVEL})
add_definitions(-LLIST_FULL_BUILD=${LLIST_FULL_BUILD})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find threading support properly
find_package(Threads REQUIRED)

# Compiler flags
add_compile_options(-Wall -Wextra -Wformat -Wpedantic -g)

# --- Library Creation ---

# Create the Core Utility Library
add_library(cllist_core STATIC 
    src/cllist.c 
    lib/logger/logger.c 
    lib/llist/llist.c
)

# Use target-based include directories (cleaner than global include_directories)
target_include_directories(cllist_core PUBLIC 
    include 
    lib/logger 
    lib/llist
)

target_link_libraries(cllist_core PRIVATE Threads::Threads)

# --- Unit Testing ---

find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(CMOCKA cmocka)
endif()

if(CMOCKA_FOUND)
    enable_testing()

    add_executable(test_suite test/test_cllist.c)

    target_include_directories(test_suite PRIVATE ${CMOCKA_INCLUDE_DIRS})
    
    # Link against the core library and cmocka
    target_link_libraries(test_suite PRIVATE 
        cllist_core 
        ${CMOCKA_LIBRARIES}
    )

    add_test(NAME cllist_unit_test COMMAND test_suite)
else()
    message(STATUS "CMocka not found. Unit tests will not be built.")
endif()
