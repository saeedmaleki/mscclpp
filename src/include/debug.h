/*************************************************************************
 * Copyright (c) 2015-2022, NVIDIA CORPORATION. All rights reserved.
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#ifndef MSCCLPP_DEBUG_H_
#define MSCCLPP_DEBUG_H_

#include "mscclpp.h"
#include <stdio.h>
#include <chrono>
#include <type_traits>

#include <limits.h>
#include <string.h>
#include <pthread.h>

// Conform to pthread and NVTX standard
#define MSCCLPP_THREAD_NAMELEN 16

typedef enum {MSCCLPP_LOG_NONE=0, MSCCLPP_LOG_VERSION=1, MSCCLPP_LOG_WARN=2, MSCCLPP_LOG_INFO=3, MSCCLPP_LOG_ABORT=4, MSCCLPP_LOG_TRACE=5} mscclppDebugLogLevel;
typedef enum {MSCCLPP_INIT=1, MSCCLPP_COLL=2, MSCCLPP_P2P=4, MSCCLPP_SHM=8, MSCCLPP_NET=16, MSCCLPP_GRAPH=32, MSCCLPP_TUNING=64, MSCCLPP_ENV=128, MSCCLPP_ALLOC=256, MSCCLPP_CALL=512, MSCCLPP_ALL=~0} mscclppDebugLogSubSys;

extern int mscclppDebugLevel;
extern uint64_t mscclppDebugMask;
extern pthread_mutex_t mscclppDebugLock;
extern FILE *mscclppDebugFile;
extern mscclppResult_t getHostName(char* hostname, int maxlen, const char delim);

void mscclppDebugLog(mscclppDebugLogLevel level, unsigned long flags, const char *filefunc, int line, const char *fmt, ...) __attribute__ ((format (printf, 5, 6)));

// Let code temporarily downgrade WARN into INFO
extern thread_local int mscclppDebugNoWarn;
extern char mscclppLastError[];

#define WARN(...) mscclppDebugLog(MSCCLPP_LOG_WARN, MSCCLPP_ALL, __FILE__, __LINE__, __VA_ARGS__)
#define INFO(FLAGS, ...) mscclppDebugLog(MSCCLPP_LOG_INFO, (FLAGS), __func__, __LINE__, __VA_ARGS__)
#define TRACE_CALL(...) mscclppDebugLog(MSCCLPP_LOG_TRACE, MSCCLPP_CALL, __func__, __LINE__, __VA_ARGS__)

#ifdef ENABLE_TRACE
#define TRACE(FLAGS, ...) mscclppDebugLog(MSCCLPP_LOG_TRACE, (FLAGS), __func__, __LINE__, __VA_ARGS__)
extern std::chrono::steady_clock::time_point mscclppEpoch;
#else
#define TRACE(...)
#endif

void mscclppSetThreadName(pthread_t thread, const char *fmt, ...);

#endif