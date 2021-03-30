/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-11
 */
#include <QApplication>

#include "lib/logger/logger.h"
#include "lib/util/pub_type.h"
#include "lib/util/util.h"
#include "mainwindow.h"
#include "common/public/pub_env.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 初始化log系统
    Logger::Instance().OpenLogger("log/kktools.log", spdlog::level::debug);
    LogInfo("Main start!");

    // 加载全局资源
    auto ret = KKpub::PubEnv::getInstance()->addResource();
    if (ret != ErrCode::RET_OK) {
        LogError("Add resource fail! ret={}", ret);
        return 1;
    }

    MainWindow w;
    w.show();
    ret = app.exec();

    Logger::Instance().CloseLogger();
    LogInfo("Main stop!");
    return ret;
}
