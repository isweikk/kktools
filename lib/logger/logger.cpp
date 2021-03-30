/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-24
 */
#include "logger.h"
//#include "stdafx.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>
Logger::Logger()
{

}

Logger::~Logger()
{
    // 这里调用drop_all，会导致程序异常退出
}

void Logger::CloseLogger()
{
    spdlog::drop_all();
}

void Logger::SetLevel(int level)
{
    myLogger_->set_level(static_cast<spdlog::level::level_enum>(level));
}

Logger& Logger::Instance()
{
    static Logger log;
    return log;
}

void Logger::OpenLogger(std::string fileName, int logLevel)
{
    //设置为异步日志
	// spdlog::set_async_mode(32768);  // 必须为 2 的幂
    std::vector<spdlog::sink_ptr> sinkList;

#ifdef _DEBUG
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::debug);
    consoleSink->set_pattern("[%m-%d %H:%M:%S.%e][%^%L%$] [Thd %t]%v");
    sinkList.push_back(consoleSink);
#endif
    auto basicSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(fileName, 0, 0, false, 5);
    basicSink->set_level(spdlog::level::debug);
    basicSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%^%L%$] [Thd %t]%v");
    sinkList.push_back(basicSink);
    myLogger_ = std::make_shared<spdlog::logger>("both", begin(sinkList), end(sinkList));
    // register it if you need to access it globally
    spdlog::register_logger(myLogger_);

#ifdef _DEBUG
    myLogger_->set_level(spdlog::level::trace);
#else
    myLogger_->set_level(static_cast<spdlog::level::level_enum>(logLevel));
#endif

    // 设置超过日志等级，立即写入文件，3秒写一次文件
    myLogger_->flush_on(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(3));

}
