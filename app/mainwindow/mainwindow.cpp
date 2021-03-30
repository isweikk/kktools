/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-11
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qevent.h>
#include <qmessagebox.h>

#include "lib/logger/logger.h"
#include "notice_view.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 加载资源文件
    Q_INIT_RESOURCE(mainwindow);
    // 加载导航栏数据
    ui->treeView->setData(":/config/nav_tree");

    // 加载推送消息窗口
    ui->noticeView->setModel(NoticeView::InfoModel({ui->noticeText, ui->noticePic}));
}

MainWindow::~MainWindow()
{
    delete ui;
    Q_CLEANUP_RESOURCE(mainwindow);
    LogDebug("{} exit", __FUNCTION__);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::warning(0, "Warning", QString("Multiple windows are opened!\n Are you sure?"),
                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        LogDebug("button yes");
        qApp->quit();
    } else if (ret == QMessageBox::Cancel) {
        LogDebug("button cancel");
        event->ignore();
    } else {
        event->ignore();
        LogDebug("button quit");
    }
}
