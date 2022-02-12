#include "util.h"

namespace MyFrame {

pid_t GetThreadId() {
    return syscall(SYS_gettid);
}

}