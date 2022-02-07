#include <iostream>
#include "src/log.h"

int main(int argc, char** argv) {
    MyFrame::Logger::ptr logger(new MyFrame::Logger);
    logger->addAppender(MyFrame::LogAppender::ptr(new MyFrame::StdoutLogAppender));

    MyFrame::LogEvent::ptr event(new MyFrame::LogEvent(__FILE__, __LINE__, 0, MyFrame::GetThreadId(), MyFrame::GetFiberId(), time(0)));
    event->getSS() << "hello sylar log";
    logger->log(MyFrame::LogLevel::DEBUG, event);
    return 0;
}