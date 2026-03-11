# Installation Guide
## Requirements:
- Compiler: C11 compatible (I used Clang)
- Build tools: CMake 3.10+ and GNU Make 4+
- OS: Ubuntu 24.04.3 LTS

## About this project:
This is my attempt at a modular linked-list library which I can use to build other data-structures. The library compiles "full" or "lean". All functionality is available with a "full" compilation, but only adding/deleting/peeking/popping is available with a "lean" compilation. I did this so that the library would be smaller (and not include any unnecessary functions) for the implementation of data structures that don't require indexing/sorting operations (stacks, queues, etc.). The library uses pointers
and dynamic memory allocation so the linked-list can "own" the data it ingests on the heap. The list is capable of injesting any 
data type, and it only relys on the caller for collaboration during "peak" and "pop" functions. The "peak" and "pop" functions
require a shared pointer to a data_size buffer that the caller must manage the memory for . 

### How It Works
1. Initialization: llist_create allocates the memory for the linked list.

2. Utilization: This library can be used as a standalone linked-list data structure, or you can use it as the foundation to create other data structures (priority queue, stack, etc.). Create a node with node_create, then add it to the linked list via llist_add_first, llist_add_last, or llist_add_index (to insert the node to a desired location in the list). The same insert locations (first, last, index) are available to all delete, peek, and pop functions. For lists that are solely numerical, there is a bubble sort function available.  

3. Teardown: The linked-list can be cleared (llist_clear) or destroyed (llist_destroy). Clearing wipes all the data in the linked-list, frees all the pointers, and maintains the main linked list to injest new data. Destroying completely frees the linked list and its pointers. 

### Default Values:
- Logging mode: 0 (no logs)

## Installation:
### 1. Clone Repository
    git clone https://github.com/bakkenld/Public-Projects.git

### 2. Navigate to correct folder
    cd C-Projects
    cd linked-list

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

### Optional build settings:
    LLIST_FULL_BUILD=1      full build (default)
    LLIST_FULL_BUILD=0      lean build (no sorting/indexing)

### Included tests
test_node_lifecycle: Testing create and destroy functions.
test_llist_clear: Validating the container still exists after clearing the data. 
test_llist_sort: Validating the numerical sort function. 
test_llist_add_index_and_delete: Testing ability to add/delete from an index. 
test_llist_peeking: Validating peek does not remove data. 
test_llist_popping: Validating pop does remove data. 
