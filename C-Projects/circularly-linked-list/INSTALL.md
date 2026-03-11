# Installation Guide
## Requirements:
- Compiler: C11 compatible (I used Clang)
- Build tools: CMake 3.10+ and GNU Make 4+
- OS: Ubuntu 24.04.3 LTS

## About this project:
This is my attempt at a circularly linked list data structure utilizing my linked list library. The circularly linked list uses a partial "full" compilation of the library so that most functions are available. The only excluded function is the sort function. Since I could not recognize a necessary reason to sort a circularly linked list. The functions for the circularly linked list are wrappers around the functions in the linked list library. The only additional functions are de_circle and re_circle. De_circle and re_circle are designed to "break" the circular logic so that the linked-list logic can be utilized, then re-circle the list after the logic is performed. The de-circle and re-circle process partially degrades the speed of list operations, but I assess the slow down to marginal since the linked-list structure includes pointers directly to the head and tail of the list (so no additional traversal is required to find new heads/tails). The circularly linked list "owns" the data it ingests on the heap. The list is capable of injesting any data type, and it only relies on the caller for collaboration during "peak" and "pop" functions. The "peak" and "pop" functions require a shared pointer to a data_size buffer that the caller must manage the memory for. 

### How It Works
1. Initialization: cllist_create allocates the memory for the list.

2. Utilization: Create nodes with node_create and add them to the list via the front (cllist_add_first), back (cllist_add_last), or index (cllist_add_index). re_circle is called to connect the tail to the head, and re_circle is called each time a node is popped or deleted from the tail of the list (to update the new tail). Use peek functions to view data without removing it, and use pop functions to utilize/remove the data from the list. 

3. Teardown: The list can be cleared (cllist_clear) or destroyed (cllist_destroy). Clearing wipes all the data in the list, frees all the pointers, and maintains the main list to injest new data. Destroying completely frees the queue and its pointers. 

### Default Values:
- Logging mode: 0 (no logs)

## Installation:
### 1. Clone Repository
    git clone https://github.com/bakkenld/Public-Projects.git

### 2. Navigate to correct folder
    cd C-Projects
    cd circularly-linked-list

### 3. Build the server (with default startup flags)
    make all

### 4. (Optional) testing
    sudo apt install libcmocka-dev
    sudo apt install pkg-config
    sudo apt install valgrind

### Additional commands (all utilize default startup flags)
    make all <optional logger flag> 
compiles the project and any tests, but does not run.

    make clean
cleans up the build directory.

    make test
compiles and executes any unit tests. 

    make valgrind
compiles and runs valgrind. (results available in the build dir)

### Optoinal logger settings
    LOG_LEVEL=0     No logs (default)
    LOG_LEVEL=1     Only error logs 
    LOG_LEVEL=2     Warning and error logs
    LOG_LEVEL=3     Success, warning, and error logs
    LOG_LEVEL=4     All logs

### Included tests:
test_cllist_insert_integrity: Tests that the tail always points back to the head after add_last/add_first. 
test_cllist_delete_and_pop: Verifies re_circle is successfully called after modifying the tail node. 
test_cllist_clear_safety: Validates there is no infinite loop while clearing the cllist. 
test_cllist_index_circularity: validates indexing functions maintain "circularity"
test_cllist_rotation: Verifies that the list can move through the entire circle.
