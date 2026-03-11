# Installation Guide

## Requirements:
- Compiler: C11 compatible (I used Clang)
- Build tools: CMake 3.10+ and GNU Make 4+
- OS: Ubuntu 24.04.3 LTS (must be POSIX-compliant enviornment)
- Dependencies: pthreads

## About this project:
This is my attempt at an "event-driven" echo-server. My intent for this server is to provide me with a foundational understanding of the interaction between poll() and a threadpool to manage multiple connections with shared resources.

### How It Works
1. Initialization (echo-server: eserver.c): Initialization is broadly managed by functions defined in eserver.c. Main.c calls these functions to start the server.

2. Socket Setup: This server listens for any connections on a default port (45454), then binds a non-blocking listener socket to it, and registers it with a poll() manager (helper-poll: dispatch.c). 

3. Work Generation: The main thread waits for an event from the dispatch array, when a client connects or sends data, the event is captured as a "task" and given to the threadpool (threadpool: tpool.c). 

4. Task Handling: Tasks are added to a queue, where a worker from tpool picks them up, processes them (their echo), returns them to the dispatch array, and returns back to tpool.

5. Teardown: Upon receiving SIGINT, the server clears all tasks, closes active sockets, and destroys the tpool. 

### Default Values:
- Utilization port: 45454 
- Number of threads: 4
- Size of poll array (initially): 8
- Connection "waitlist": 10
- Maximum connections: 256 
- Logging mode: No logs 

## Installation:
### 1. Clone Repository
    git clone https://github.com/bakkenld/Public-Projects.git

### 2. Navigate to correct folder
    cd C-Projects
    cd echo-server

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

    make helgrind
compiles and runs helgrind. (results available in the build dir)

    make run <optional logger flag>
compiles everything and runs the server.

### Optoinal logger settings
    LOG_LEVEL=0     No logs (default)
    LOG_LEVEL=1     Only error logs 
    LOG_LEVEL=2     Warning and error logs
    LOG_LEVEL=3     Success, warning, and error logs
    LOG_LEVEL=4     All logs

## Usage:
### 1. To start the server with default flags, follow steps 1-4 outlined above, then:
    cd build
    ./echo-server
### 1. (Alternative) follow steps 1-3 outlined above, then:
    make run
    (startup flags are not supported with make run)
### 1. (Alternative) To start the server with personalized flags, follow steps 1-4, then:
    cd build
    ./echo-server -p <1024-65535>
    (-h is available for help)
### 2. By default, the server will operate on port 45454 of the machine it is ran on. The server will echo strings received on port 45454. I used Netcat to connect to the server. (In a separate terminal) Run:
    nc localhost <port(45454 by default)>
### 3. The server will then echo any input directly back to the client. The string should appear in both the client and servers terminal when it is echoed. 
    hello <hello>
### 4. Disconnect the client at any time with ^C (Control+C) from the client's terminal
    ctrl+C
### 5. Stop the server at any time with ^C (Control+C) from the server's terminal
    ctrl+C