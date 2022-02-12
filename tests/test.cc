#include <iostream>
#include "../src/log.h"
#include "../src/util.h"

int main(int argc, char** argv) {
    MyFrame::Logger::ptr logger(new MyFrame::Logger);
    logger->addAppender(MyFrame::LogAppender::ptr(new MyFrame::StdoutLogAppender));

    MyFrame::LogEvent::ptr event(new MyFrame::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));
    event->getSS() << "hello my log";

    logger->log(MyFrame::LogLevel::DEBUG, event);
    std::cout << "hello my log" << std::endl;
    return 0;
}