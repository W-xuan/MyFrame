#include <iostream>
#include "../src/log.h"
#include "../src/util.h"

int main(int argc, char** argv) {
    MyFrame::Logger::ptr logger(new MyFrame::Logger);
    logger->addAppender(MyFrame::LogAppender::ptr(new MyFrame::StdoutLogAppender));

    MyFrame::FileLogAppender::ptr file_appender(new MyFrame::FileLogAppender("./log.txt"));
    logger->addAppender(file_appender);

    std::cout << "Hello My log" << std::endl;

    MYFRAME_LOG_INFO(logger) << "test macro";
    MYFRAME_LOG_ERROR(logger) << "test macro error";

    MYFRAME_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
    return 0;
}