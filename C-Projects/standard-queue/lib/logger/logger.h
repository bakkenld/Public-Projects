/**
 * @file logger.h
 * @author Logan Bakken
 * @brief Logger that utilizes pre-compilation statements to provide logging 
 * levels to programs
 * @version 0.2
 * @date 2026-02-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

// Logging levels available
#define LOG_LEVEL_NONE      0 // no logs
#define LOG_LEVEL_ERR       1 
#define LOG_LEVEL_WARN      2 
#define LOG_LEVEL_SUCCESS   3 
#define LOG_LEVEL_DBG       4 // all logs

// Provided by startup flag (default is none)
#ifndef TARGET_LOG_LEVEL
    #define TARGET_LOG_LEVEL LOG_LEVEL_NONE
#endif

#if TARGET_LOG_LEVEL >= LOG_LEVEL_ERR 
    #define L_ERR(fmt, ...) logger_log(LOG_ERR, fmt, ##__VA_ARGS__)
#else
    #define L_ERR(fmt, ...) ((void)0)
#endif

#if TARGET_LOG_LEVEL >= LOG_LEVEL_WARN
    #define L_WARN(fmt, ...) logger_log(LOG_WARN, fmt, ##__VA_ARGS__)
#else
    #define L_WARN(fmt, ...) ((void)0)
#endif

#if TARGET_LOG_LEVEL >= LOG_LEVEL_SUCCESS
    #define L_SUCCESS(fmt, ...) logger_log(LOG_SUCCESS, fmt, ##__VA_ARGS__) 
#else
    #define L_SUCCESS(fmt, ...) ((void)0)
#endif

#if TARGET_LOG_LEVEL >= LOG_LEVEL_DBG //HERE NOW
    #define L_DBG(fmt, ...) logger_log(LOG_DBG, fmt, ##__VA_ARGS__)
#else
    #define L_DBG(fmt, ...) ((void)0)
#endif

typedef enum {
    LOG_DBG,
    LOG_SUCCESS,
    LOG_WARN,
    LOG_ERR,
    LOG_LEVEL_LEN
} LOG_LEVEL;

extern char * type[LOG_LEVEL_LEN];

extern char * colors[LOG_LEVEL_LEN];

#ifdef __EMSCRIPTEN
    #define NO_ANSI
#endif

void get_time(struct tm * const p_time_out);

void logger_log(LOG_LEVEL level, const char * fmt, ...);

#endif // LOGGER_H

// End of file
