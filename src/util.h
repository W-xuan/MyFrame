#ifndef __MYFRAME_UTIL_H__
#define __MYFRAME_UTIL_H__

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>

namespace MyFrame {

pid_t GetThreadId();



}

#endif