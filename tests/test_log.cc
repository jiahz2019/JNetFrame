//@brief 日志类测试

#include "src/jhz.h"

#include <unistd.h>

jhz::Logger::ptr g_logger = JHZ_LOG_ROOT(); // 默认INFO级别

int main(int argc, char *argv[]) {
    jhz::EnvMgr::GetInstance()->init(argc, argv);
    jhz::Config::LoadFromConfDir(jhz::EnvMgr::GetInstance()->getConfigPath(),true);


    JHZ_LOG_FATAL(g_logger) << "fatal msg";
    JHZ_LOG_ERROR(g_logger) << "err msg";
    JHZ_LOG_INFO(g_logger) << "info msg";
    JHZ_LOG_DEBUG(g_logger) << "debug msg";

    JHZ_LOG_FMT_FATAL(g_logger, "fatal %s:%d", __FILE__, __LINE__);
    JHZ_LOG_FMT_ERROR(g_logger, "err %s:%d", __FILE__, __LINE__);
    JHZ_LOG_FMT_INFO(g_logger, "info %s:%d", __FILE__, __LINE__);
    JHZ_LOG_FMT_DEBUG(g_logger, "debug %s:%d", __FILE__, __LINE__);
   
    sleep(1);
    jhz::SetThreadName("brand_new_thread");

    g_logger->setLevel(jhz::LogLevel::WARN);
    JHZ_LOG_FATAL(g_logger) << "fatal msg";
    JHZ_LOG_ERROR(g_logger) << "err msg";
    JHZ_LOG_INFO(g_logger) << "info msg"; // 不打印
    JHZ_LOG_DEBUG(g_logger) << "debug msg"; // 不打印


    jhz::FileLogAppender::ptr fileAppender(new jhz::FileLogAppender("./log/log.txt"));
    g_logger->addAppender(fileAppender);
    JHZ_LOG_FATAL(g_logger) << "fatal msg";
    JHZ_LOG_ERROR(g_logger) << "err msg";
    JHZ_LOG_INFO(g_logger) << "info msg"; // 不打印
    JHZ_LOG_DEBUG(g_logger) << "debug msg"; // 不打印

    jhz::Logger::ptr test_logger = JHZ_LOG_NAME("test_logger");
    jhz::StdoutLogAppender::ptr appender(new jhz::StdoutLogAppender);
    jhz::LogFormatter::ptr formatter(new jhz::LogFormatter("%d:%rms%T%p%T%c%T%f:%l %m%n")); // 时间：启动毫秒数 级别 日志名称 文件名：行号 消息 换行
    appender->setFormatter(formatter);
    test_logger->addAppender(appender);
    test_logger->setLevel(jhz::LogLevel::WARN);

    JHZ_LOG_ERROR(test_logger) << "err msg";
    JHZ_LOG_INFO(test_logger) << "info msg"; // 不打印

    // 输出全部日志器的配置
    g_logger->setLevel(jhz::LogLevel::INFO);
    JHZ_LOG_INFO(g_logger) << "logger config:" << jhz::LoggerMgr::GetInstance()->toYamlString();
    

    return 0;
}