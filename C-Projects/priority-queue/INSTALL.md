# Installation Guide
## Requirements:
- Compiler: C11 compatible (I used Clang)
- Build tools: CMake 3.10+ and GNU Make 4+
- OS: Ubuntu 24.04.3 LTS

## About this project:
This is my attempt at a priority queue data structure utilizing my linked list library. The priority queue uses a "lean" compilation of the library so that unnecessary functions are unavailable. The functions for the priority queue are wrappers around the functions in the linked list library. The difference between the standard-queue and the priority-queue is that the priority-queue uses llist_add_index within pqueue_push to insert nodes to the proper index by priority value. The priority queue "owns" the data it ingests on the heap. The queue is capable of injesting any data type, and it only relys on the caller for collaboration during "peak" and "pop" functions. The "peak" and "pop" functions require a shared pointer to a data_size buffer that the caller must manage the memory for. 

### How It Works
1. Initialization: pqueue_create allocates the memory for the queue.

2. Utilization: Create nodes with node_create and add them to the queue with pqueue_push. Use pqueue_peek to view data without removing it from the queue, and use pqueue_pop to utilize/remove the data from the queue. 

3. Teardown: The queue can be cleared (pqueue_clear) or destroyed (pqueue_destroy). Clearing wipes all the data in the queue, frees all the pointers, and maintains the main queue to injest new data. Destroying completely frees the queue and its pointers. 

### Default Values:
- Logging mode: 0 (no logs)

## Installation:
### 1. Clone Repository
    git clone https://github.com/bakkenld/Public-Projects.git

### 2. Navigate to correct folder
    cd C-Projects
    cd priority-queue

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
test_pqueue_priority_ordering: Validating the priority queues ability to injest data with priority. 
test_pqueue_peek: Testing that the peeked value is the value with highest priority. 
test_pqueue_empty: NULL test against pop and peek functions. 
