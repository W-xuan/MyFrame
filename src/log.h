#ifndef __MYFRAME_LOG_H__
#define __MYFRAME_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>

// 使用流式方式将日志级别level的日志写入到logger
#define MYFRAME_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
        MyFrame::LogEventWrap(MyFrame::LogEvent::ptr(new MyFrame::LogEvent(logger, level, \
                            _FILE_, _LINE_, 0, MyFrame::GetThreadId(),\
                        MyFrame::GetFiberId(), time(0), MyFrame::Thread::GetName()))).getss()

// 使用流式方式将日志级别debug的日志写入到logger
#define MYFRAME_LOG_DEBUG(logger) MYFRAME_LOG_LEVEL(logger, MyFrame::LogLevel::DEBUG)

// 使用流式方式将日志级别info的日志写入到logger
#define MYFRAME_LOG_INFO(logger) MYFRAME_LOG_LEVEL(logger, MyFrame::LogLevel::INFO)

// 使用流式方式将日志级别warn的日志写入到logger
#define MYFRAME_LOG_WARN(logger) MYFRAME_LOG_LEVEL(logger, MyFrame::LogLevel::WARN)

// 使用流式方式将日志级别error的日志写入到logger
#define MYFRAME_LOG_ERROR(logger) MYFRAME_LOG_LEVEL(logger, MyFrame::LogLevel::ERROR)

// 使用流式方式将日志级别fatal的日志写入到logger
#define MYFRAME_LOG_FATAL(logger) MYFRAME_LOG_LEVEL(logger, MyFrame::LogLevel::FATAL)

namespace MyFrame {

class Logger;
class LoggerManager;

//日志级别
class LogLevel {
public:
    enum Level {
        // 未知级别
        UNKOWN = 0,
        // DEBUG级别
        DEBUG = 1,
        // INFO级别
        INFO = 2,
        // WARN级别
        WARN = 3,
        // ERROR级别
        ERROR = 4,
        // FATAL级别
        FATAL = 5,
    };

    // 将日志级别转换成文本输出
    static const char* ToString(LogLevel::Level level);
    // 将文本转换成日志级别
    static LogLevel::Level FromString(const std::string& str);
};

//日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    // 构造函数
    LogEvent(const char* file, int32_t m_line, uint32_t getElapse
            , uint32_t thread_id, uint32_t fiber_id, uint64_t time);
    // 返回文件名
    const char* getFile() const { return m_file; }
    
    // 返回行号
    int32_t getLine() const { return m_line; }
    
    // 返回耗时
    uint32_t getElapse() const { return m_elapse; }
    
    // 返回线程ID
    uint32_t getThreadId() const { return m_threadId; }
    
    // 返回协程ID
    uint32_t getFiberId() const { return m_fiberId; }
    
    // 返回时间
    uint64_t getTime() const { return m_time; }
    
    // 返回线程名称
    std::string getContent() const { return m_ss.str(); }

    // 返回日志器
    std::shared_ptr<Logger> getLogger() const { return m_logger; } 

    //返回日志级别
    LogLevel::Level getLevel() const { return m_level; }
    
    // 返回日志内容字符串流
    std::stringstream& getSS() { return m_ss; }
private:
    //文件名
    const char* m_file = nullptr; 
    
    //行号
    int32_t m_line = 0;     
    
    //程序启动开始到现在的毫秒数
    uint32_t m_elapse = 0;   
    
    //线程ID
    uint32_t m_threadId = 0;  
    
    //协程ID
    uint32_t m_fiberId = 0;       
    
    //时间戳
    uint32_t m_time = 0;              
    
    //日志内容流
    std::stringstream m_ss;   
    
    //日志器
    std::shared_ptr<Logger> m_logger;

    // 日志等级
    LogLevel::Level m_level;
     
};

//日志事件包装器
class LogEventWrap {
public:
    // 构造函数
    // e 日志事件
    LogEventWrap(LogEvent::ptr e);

    // 析构函数
    ~LogEventWrap();

    // 获取日志事件
    LogEvent::ptr getEvent() const { return m_event; }

    // 获取日志内容流
    std::stringstream& getss();
private:
    // 日志事件
    LogEvent::ptr m_event;
};

//日志格式器
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);
    //%t    %thred_id %m%n
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger>, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr; 
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0; 
    };

    void init();
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error = false;
};

//日志输出地
class LogAppender {
friend class Logger;
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr val);
    LogFormatter::ptr getFormatter();
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }
protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    LogFormatter::ptr m_formatter;
};

//日志器
class Logger : public std::enable_shared_from_this<Logger> {
friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

    const std::string& getName() const { return m_name; }
private:
    // 日志名称
    std::string m_name;
    // 日志级别        
    LogLevel::Level m_level;
    // 日志目标集合      
    std::list<LogAppender::ptr> m_appenders;
    // 日志格式器    
    LogFormatter::ptr m_formatter;
    // 主日志器
    Logger::ptr m_root;
};

//输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
};

//输出到文件的Appender
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

    //重新打开文件, 文件打开成功返回true
    bool reopen();
private:
    std::string m_filename; 
    std::ofstream m_filestream;
};

// 日志器管理类
class LoggerManager {
public:
    //构造函数
    LoggerManager();

    //获取日志器
    Logger::ptr getLogger(const std::string& name);

    // 初始化
    void init();

    // 返回主日志器
    Logger::ptr getRoot() const { return m_root; }
private:
    // 日志器容器
    std::map<std::string, Logger::ptr> m_loggers;
    // 主日志器
    Logger::ptr m_root;
};

}

#endif