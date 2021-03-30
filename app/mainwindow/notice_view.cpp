/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-22
 */
#include "notice_view.h"

#include <QMessageBox>
#include <qimage.h>
#include <qevent.h>
#include "lib/logger/logger.h"
#include "common/public/pub_env.h"
#include "common/public/pub_util.h"

using namespace KKpub;

NoticeView::NoticeView(QWidget *parent) : QWidget(parent)
{
}

NoticeView::~NoticeView()
{
    LogDebug("{} exit", __FUNCTION__);
}

void NoticeView::initData()
{
    LogInfo("Start load the notice window");
    // 把所有照片的路径都放到QStringList里去.
    for (int i = 0; i < 5; ++i) {
        QString path = ":/image/notice_" + QString::number(i);
        if (PubUtil::isFileExist(path)) {
            picList.push_back(path);
        }
    }
    LogInfo("load image total={0}", picList.size());

    // 文本框填充
    textList.append(QStringLiteral("捐赠，加速开发"));
    textList.append(QStringLiteral("关注，反馈意见"));
    textList.append(QStringLiteral("展览，个人作品"));
    textList.append("none");
    textList.append("none");
}

void NoticeView::setModel(InfoModel infoModel) {
    initData();
    info.textLabel = infoModel.textLabel;   // 这里要用深拷贝，避免空指针问题
    info.picLabel = infoModel.picLabel;
    changeNoticeView();
}

void NoticeView::mousePressEvent(QMouseEvent* event)
{
    LogInfo("Start Event");
    if (event->button()== Qt::LeftButton) {
        changeNoticeView();
    }
}

void NoticeView::changeNoticeView()
{
    static int32_t picCnt = 0;
    QPixmap img;

    if (picCnt >= picList.size()) {
        picCnt = 0;
    }
    for (; picCnt < picList.size(); picCnt++) {
        auto ret = img.load(picList[picCnt]);
        if (ret) {
            // 标题显示
            info.textLabel->setStyleSheet("color:blue");
            info.textLabel->setFont(QFont("Microsoft YaHei", 10, 75));
            info.textLabel->setAlignment(Qt::AlignCenter);
            info.textLabel->setText(textList.at(picCnt));
            // 图片显示
            img = img.scaled(info.picLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);   // 调整图片为拉伸
            info.picLabel->setPixmap(img);
            picCnt++;
            break;
        }
    }
}
