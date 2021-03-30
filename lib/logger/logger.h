/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-24
 */
#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"

#ifdef _WIN32
// strrchr:查找字符在指定字符串从右面开始的第一次出现的位置，如果成功，返回该字符以及后面的字符，如果失败，返回NULL
// strcgr:查找字符在指定字符串首次出现的位置
#define __FILENAME__ (strrchr(__FILE__,'\\')?(strrchr(__FILE__,'\\')+1):__FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__,'/')?(strrchr(__FILE__,'/')+1):__FILE__)
#endif // _WIN32

#ifndef PREFIX
// 在错误级别的日志后面追加文件名，函数名，行号
#define PREFIX(msg) std::string().append(" [")\
    .append(__FILENAME__).append(" :").append(std::to_string(__LINE__))\
    .append("] ").append(std::string(msg)).c_str()
#endif // PREFIX

#ifndef SUFFIX
// 在错误级别的日志后面追加文件名，函数名，行号
#define SUFFIX(msg) std::string(msg).append("  <")\
    .append(__FILENAME__).append("> <").append(__FUNCTION__)\
    .append("> <").append(std::to_string(__LINE__))\
    .append(">").c_str()
#endif // suffix

#define LogTrace(msg, ...)  Logger::Instance().GetLogger()->trace(PREFIX(msg), __VA_ARGS__)
#define LogDebug(msg, ...)  Logger::Instance().GetLogger()->debug(PREFIX(msg), __VA_ARGS__)
#define LogInfo(msg, ...)  Logger::Instance().GetLogger()->info(PREFIX(msg), __VA_ARGS__)
#define LogWarn(msg, ...) Logger::Instance().GetLogger()->warn(PREFIX(msg), __VA_ARGS__)
//#define LogError(msg, ...)  Logger::Instance().GetLogger()->error(SUFFIX(msg),__VA_ARGS__)
#define LogError(msg, ...)  Logger::Instance().GetLogger()->error(PREFIX(msg), __VA_ARGS__)
#define LogCritical(msg, ...)  Logger::Instance().GetLogger()->critical(PREFIX(msg), __VA_ARGS__)

class Logger {
private:
    std::shared_ptr<spdlog::logger> myLogger_;

public:
    static Logger &Instance();
    void OpenLogger(std::string fileName, int logLevel = spdlog::level::trace);
    void CloseLogger();

    void SetLevel(int level = spdlog::level::trace);

    auto GetLogger()->decltype(myLogger_)
    {
        return myLogger_;
    }

private:
    Logger();
    ~Logger();
    Logger(const Logger &other) = delete;
    Logger& operator=(const Logger &other) = delete;
};

#endif // LOGGER_H
