/**
 * @file logger.c
 * @author Logan Bakken
 * @brief Logger from hash-table
 * @version 0.1
 * @date 2026-01-26
 * 
 * @copyright Copyright (c) 2026
 * 
 */

 #include "logger.h"

char * type[LOG_LEVEL_LEN] = {
    "DEBUG",
    "SUCCESS",
    "WARN",
    "ERR"
}; 

char * colors[LOG_LEVEL_LEN] = {
    "\x1b[0m", // white
    "\x1b[32m", // green
    "\x1b[1;33m", // yellow
    "\x1b[31m" // red
};

/**
 * @brief Thread-safe time retrieval
 * 
 * @param p_time_out Pointer to a local struct tm to be filled
 */
void get_time(struct tm * const p_time_out) 
{
    time_t current_time = 0;
    
    if (NULL != p_time_out)
    {
        time(&current_time);
        
        if (NULL == localtime_r(&current_time, p_time_out))
        {
            return; // p_time_out is initialized to {0}
        }
    }
}

/**
 * @brief Function to log event
 * 
 * @param level DBG, SUCCESS, WAR, ERROR
 * @param fmt What you would like to say
 * @param ... additional arguments
 */
void logger_log(LOG_LEVEL level, const char * fmt, ...) 
{
    struct tm local_m_time = {0}; 
    va_list args = {0};

    if (level >= LOG_LEVEL_LEN) 
    {
        return;
    }

    get_time(&local_m_time);

    va_start(args, fmt);
        #ifndef NO_ANSI 
            printf("%s", colors[level]);
        #endif
        
        printf("[%d/%02d/%d -> %02d:%02d:%02d][%s] ", 
                local_m_time.tm_mday,
                local_m_time.tm_mon + 1,
                local_m_time.tm_year + 1900,
                local_m_time.tm_hour, 
                local_m_time.tm_min, 
                local_m_time.tm_sec,
                type[level]); 

        vfprintf(stdout, fmt, args);

        #ifndef NO_ANSI
            // Reset color
            printf("\x1b[0m\n");
        #else
            printf("\n");
        #endif
    va_end(args);
}



// End of file
