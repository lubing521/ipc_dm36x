// Copyright (c) 2012 tongfangcloud, Inc.  All rights reserved.


#ifndef __DBG__
#define __DBG__

#include <stdio.h>
#include <stdlib.h>

#define NONE        "\033[0m"
#define BLACK       "\033[0;30m"
#define DARK_GRAY   "\033[1;30m"
#define BLUE        "\033[0;34m"
#define LIGHT_BLUE  "\033[1;34m"
#define GREEN       "\033[0;32m"
#define LIGHT_GREEN "\033[1;32m"
#define CYAN        "\033[0;36m"
#define LIGHT_CYAN  "\033[1;36m"
#define RED         "\033[0;31m"
#define LIGHT_RED   "\033[1;31m"
#define PURPLE      "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN       "\033[0;33m"
#define YELLOW      "\033[1;33m"
#define LIGHT_GRAY  "\033[0;37m"
#define WHITE       "\033[1;37m"

typedef enum
{
    DBG_STD_LOG, /*stdout*/
    DBG_SYS_LOG, /*syslog, for Unix only*/
    DBG_FILE_LOG /*file output*/
} DBG_DEST;


#define PRINT(fmt, arg...)  printf(fmt, ##arg)


#define LOGLEVEL_ERR   1
#define LOGLEVEL_WARN  2
#define LOGLEVEL_DEBUG 3
#define LOGLEVEL_INFO  4

extern int loglevel;

#define ERR(fmt, arg...)\
    do\
    {\
        if ((loglevel & 0xffff) >= LOGLEVEL_ERR)\
        {\
            PRINT(RED"[%s:%s..%d]:"NONE, __FILE__, __FUNCTION__, __LINE__);\
            PRINT(fmt, ##arg);\
        }\
    } while(0)

#define WARN(fmt, arg...)\
    do\
    {\
        if ((loglevel & 0xffff) >= LOGLEVEL_WARN)\
        {\
            PRINT(YELLOW"[%s:%s..%d]:"NONE, __FILE__, __FUNCTION__, __LINE__);\
            PRINT(fmt, ##arg);\
        }\
    } while(0)

#define DBG(fmt, arg...)\
    do\
    {\
        if ((loglevel & 0xffff) >= LOGLEVEL_DEBUG)\
        {\
            PRINT(GREEN"[%s:%s..%d]:"NONE, __FILE__, __FUNCTION__, __LINE__);\
            PRINT(fmt, ##arg);\
        }\
    } while(0)

#define INFO(fmt, arg...)\
    do \
    { \
        if ((loglevel&0xffff) >= LOGLEVEL_INFO) \
        { \
            PRINT(GREEN"[%s:%s..%d]:"NONE, __FILE__, __FUNCTION__, __LINE__); \
            PRINT(fmt, ##arg); \
        } \
    } while(0)

#define PRI(fmt, arg...)\
    do \
    { \
        { \
            PRINT(GREEN"[%s:%s..%d]:"NONE, __FILE__, __FUNCTION__, __LINE__); \
            PRINT(fmt, ##arg); \
        } \
    } while(0)

#define ASSERT(x)\
    do \
    {\
        if(!(x))\
        {\
            ERR("assert expression: %s,at file:%s,line:%d\n",#x,__FILE__,__LINE__);\
        }\
    } while(0)

#define HALT()\
    do\
    {\
        ERR("halt\n");\
        abort();\
    } while(0)

#define PERROR(s) \
    do\
    {\
        ERR("perror\n");\
        perror(s);\
    } while(0)

#ifndef ENTER
#define ENTER printf("%s enter\n", __FUNCTION__)
#endif

#ifndef EXIT
#define EXIT printf("%s exit\n", __FUNCTION__)
#endif

#if _DEBUG_
#define _TRACE         printf("[ %s, %d ]=> ",__FILE__,  __LINE__);printf("\r\n");
#define _DEBUG(msg...) printf("[ %s, %d ]=> ",__FILE__,  __LINE__);printf(msg);printf("\r\n");
#define _ERROR(msg...) fprintf(stderr, "[ %s, %d ]=> ",__FILE__,  __LINE__);printf(msg);printf("\r\n")
#define _ASSERT(exp...) assert(exp)
#else
#define _TRACE
#define _DEBUG(msg...)  (void)(msg)
#define _ASSERT(exp...) (void)(exp)
#endif

#define return_if_fail(expr)         if (!(expr)) {printf("%s:%i- assertion"#expr "failed\n",__FILE__,__LINE__); return;}
#define return_val_if_fail(expr,ret) if (!(expr)) {printf("%s:%i- assertion" #expr "failed\n",__FILE__,__LINE__); return (ret);}

#ifndef LOG_MODULE
#define LOG_MODULE __FILE__
#endif /* LOG_MODULE */

/* time measuring macros for profiling tasks */
#ifdef DEBUG
#define _PROFILE(function)                                                \
    do {                                                                   \
        struct timeval current_time;                                         \
        double dtime;                                                        \
        gettimeofday(&current_time, NULL);                                   \
        dtime = -(current_time.tv_sec + (current_time.tv_usec / 1000000.0)); \
        function;                                                            \
        gettimeofday(&current_time, NULL);                                   \
        dtime += current_time.tv_sec + (current_time.tv_usec / 1000000.0);   \
        printf("%s: (%s:%d) took %lf seconds\n",                             \
               LOG_MODULE, __FUNCTION__, __LINE__, dtime);                   \
    } while(0)
#define _PROFILE_ACCUMULATE(function)                                     \
    do {                                                                   \
        struct timeval current_time;                                         \
        static double dtime = 0;                                             \
        gettimeofday(&current_time, NULL);                                   \
        dtime -= current_time.tv_sec + (current_time.tv_usec / 1000000.0);   \
        function;                                                            \
        gettimeofday(&current_time, NULL);                                   \
        dtime += current_time.tv_sec + (current_time.tv_usec / 1000000.0);   \
        printf("%s: (%s:%d) took %lf seconds\n",                             \
               LOG_MODULE, __FUNCTION__, __LINE__, dtime);                   \
    } while(0)
#endif /* DEBUG */

#endif
