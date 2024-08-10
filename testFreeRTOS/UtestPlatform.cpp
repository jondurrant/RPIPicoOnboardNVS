/*
 * Port for FreeRTOS based on the GCC example
 */

#include "FreeRTOS.h"
#include <stdlib.h>
#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc
#undef strdup
#undef strndup

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>

#include "pico/stdlib.h"


#include "CppUTest/PlatformSpecificFunctions.h"

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

static void RtosPlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin*, TestResult* result)
{
    result->addFailure(TestFailure(shell, "-p doesn't work on this platform, as it is lacking fork.\b"));
}

static int PlatformSpecificForkImplementation(void)
{
    return 0;
}

static int PlatformSpecificWaitPidImplementation(int, int*, int)
{
    return 0;
}



TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        RtosPlatformSpecificRunTestInASeperateProcess;
int (*PlatformSpecificFork)(void) = PlatformSpecificForkImplementation;
int (*PlatformSpecificWaitPid)(int, int*, int) = PlatformSpecificWaitPidImplementation;

extern "C" {

static int PlatformSpecificSetJmpImplementation(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

/*
 * MacOSX clang 3.0 doesn't seem to recognize longjmp and thus complains about _no_return_.
 * The later clang compilers complain when it isn't there. So only way is to check the clang compiler here :(
 */
#ifdef __clang__
 #if !((__clang_major__ == 3) && (__clang_minor__ == 0))
 _no_return_
 #endif
#endif
static void PlatformSpecificLongJmpImplementation()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void PlatformSpecificRestoreJumpBufferImplementation()
{
    jmp_buf_index--;
}

void (*PlatformSpecificLongJmp)() = PlatformSpecificLongJmpImplementation;
int (*PlatformSpecificSetJmp)(void (*)(void*), void*) = PlatformSpecificSetJmpImplementation;
void (*PlatformSpecificRestoreJumpBuffer)() = PlatformSpecificRestoreJumpBufferImplementation;

///////////// Time in millis

static long unsigned int TimeInMillisImplementation()
{
	return to_ms_since_boot(get_absolute_time());
}

static const char* TimeStringImplementation()
{
    time_t theTime = time(NULLPTR);
    static char dateTime[80];
#ifdef STDC_WANT_SECURE_LIB
    static struct tm lastlocaltime;
    localtime_s(&lastlocaltime, &theTime);
    struct tm *tmp = &lastlocaltime;
#else
    struct tm *tmp = localtime(&theTime);
#endif
    strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
    return dateTime;
}

long unsigned int (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

/* Wish we could add an attribute to the format for discovering mis-use... but the __attribute__(format) seems to not work on va_list */
#ifdef __clang__
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#endif
int (*PlatformSpecificVSNprintf)(char *str, size_t size, const char* format, va_list va_args_list) = vsnprintf;

static PlatformSpecificFile PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
#ifdef STDC_WANT_SECURE_LIB
  FILE* file;
   fopen_s(&file, filename, flag);
   return file;
#else
   return fopen(filename, flag);
#endif
}

static void PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
   fputs(str, (FILE*)file);
}

static void PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
   fclose((FILE*)file);
}

static void PlatformSpecificFlushImplementation()
{
  fflush(stdout);
}

PlatformSpecificFile PlatformSpecificStdOut = stdout;

PlatformSpecificFile (*PlatformSpecificFOpen)(const char*, const char*) = PlatformSpecificFOpenImplementation;
void (*PlatformSpecificFPuts)(const char*, PlatformSpecificFile) = PlatformSpecificFPutsImplementation;
void (*PlatformSpecificFClose)(PlatformSpecificFile) = PlatformSpecificFCloseImplementation;

void (*PlatformSpecificFlush)() = PlatformSpecificFlushImplementation;

void * rtosRealloc(void* p, size_t n){
	vPortFree(p);
	return pvPortMalloc(n);
}

void* (*PlatformSpecificMalloc)(size_t size) = pvPortMalloc;
void* (*PlatformSpecificRealloc)(void*, size_t) = rtosRealloc;
void (*PlatformSpecificFree)(void* memory) = vPortFree;
void* (*PlatformSpecificMemCpy)(void*, const void*, size_t) = memcpy;
void* (*PlatformSpecificMemset)(void*, int, size_t) = memset;

/* GCC 4.9.x introduces -Wfloat-conversion, which causes a warning / error
 * in GCC's own (macro) implementation of isnan() and isinf().
 */
#if defined(__GNUC__) && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8))
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

static int IsNanImplementation(double d)
{
    return isnan(d);
}

static int IsInfImplementation(double d)
{
    return isinf(d);
}

double (*PlatformSpecificFabs)(double) = fabs;
void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(void(*func)(void)) = atexit;  /// this was undefined before


static PlatformSpecificMutex PThreadMutexCreate(void)
{
    return NULLPTR;
}
static void PThreadMutexLock(PlatformSpecificMutex)
{
}
static void PThreadMutexUnlock(PlatformSpecificMutex)
{
}
static void PThreadMutexDestroy(PlatformSpecificMutex)
{
}

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = PThreadMutexCreate;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = PThreadMutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = PThreadMutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = PThreadMutexDestroy;
void (*PlatformSpecificAbort)(void) = abort;

}
