/**
 * @file sinput.c
 * @author Logan Bakken
 * @brief Functions for taking user input to start echo-server
 * @version 0.1
 * @date 2026-02-06
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "startup.h"

/**
 * @brief Function to validate whether a provided port is valid for a server. 
 * 
 * @param p_is_port pointer to the string port number 
 * @return int -1 on failure, the int port value on success
 */
static int validate_port(const char *p_is_port)
{
    int return_val = -1;
    char *p_end = NULL;
    long port = strtol(p_is_port, &p_end, 10);

    if (p_end == p_is_port)
    {
        printf("No port number provided\nUse '-h' for help");
        goto EXIT;
    }

    if ((LONG_MAX == port || LONG_MIN == port) || (0 != return_val && \
        0 == port))
    {
        printf("Too much information provided\nUse '-h' for help");
        goto EXIT;
    }
    
    if ('\0' != *p_end)
    {
        printf("Invalid port format [check for non-ints]\nUse '-h' for help");
        goto EXIT;
    }

    if (1024 > port || 65535 < port)
    {
        printf("Port is outside of acceptable range\nUse '-h' for help");
        goto EXIT;
    }

    return_val = (int)port;

EXIT:
    return return_val;
}

/**
 * @brief Function to process input provided when calling a server
 * 
 * @param argc main() input
 * @param argv main() input 
 * @param p_config pointer to a server config 
 * @return int -1 on failure, 1 on -h, 0 on no input provided
 */
int recv_input(int argc, char *argv[], server_config_t *p_config)
{
    int opt = 0;
    int return_val = -1;

    p_config->p_myport = "45454";
    p_config->waitlist = 10;
    p_config->num_threads = 4;
    p_config->dispatch_size = 8;

    while (-1 != (opt = getopt(argc, argv, "p:h")))
    {
        switch(opt)
        {
            case 'p':
                if (-1 == validate_port(optarg))
                {
                    printf("\nPlease try again\n");
                    return return_val;
                }
                p_config->p_myport = optarg;
                break;
            case 'h':
                printf("Server initialization usage:\nPort selection:" \
                    "'-p [PORT: 1024-65535]'\nAsk for help: -h\n");
                    return_val = 1;
                    return return_val;
                break;
            default:
                return return_val;
        }
    }
    return_val = 0;
    return return_val;
}

// End of file
