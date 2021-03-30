/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-21
 */
#include "pub_env.h"

#include <QFontDatabase>
#include "lib/logger/logger.h"

using namespace KKpub;

// 初始化内部似有类的静态变量，目的是启动删除的析构函数，如果不初始化，就不会被析构
// 内部类可以访问外部类的私有成员，外部类不能访问内部类的私有成员！
PubEnv::Garbage PubEnv::garbage;
PubEnv *PubEnv::instance = nullptr;

uint32_t PubEnv::addResource()
{
    Q_INIT_RESOURCE(common);
    getIconFont();
    return 0;
}

void PubEnv::delResource()
{
    QFontDatabase::removeAllApplicationFonts();
    Q_CLEANUP_RESOURCE(common);
}

QFont &PubEnv::getIconFont()
{
    static int fontId = -1;
    if (fontId == -1) {
        fontId = QFontDatabase::addApplicationFont(QString(":/common/iconfont_s1"));
    }
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    this->iconFont = QFont(fontName);

    return iconFont;
}
