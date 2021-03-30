/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-19
 */
#ifndef NAV_TREE_MODEL_H
#define NAV_TREE_MODEL_H

#include <QAbstractListModel>
#include <vector>

#include "lib/logger/logger.h"

class NavTreeModel : public QAbstractListModel {
    Q_OBJECT
public:
    NavTreeModel(QObject *parent) {};
    ~NavTreeModel();

    struct TreeNode {
        int level;                       // 层级,父节点-1,子节点-2
        bool collapse;                   // 折叠标记
        bool last;                       // 是否末尾元素
        QChar icon;                      // 左侧图标
        QString label;                   // 显示的节点文字
        QString tip;                     // 右侧描述文字
        QString parentLabel;             // 父节点名称
        std::list<TreeNode *> children;  // 子节点集合
    };

    struct ViewNode {
        QString label;       // 节点文字
        TreeNode *treeNode;  // 节点指针
    };

    void readDataFromFile(const QString path);

protected:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

public Q_SLOTS:
    void setData(const QStringList &listItems);
    void collapse(const QModelIndex &index);

private:
    void refreshList();

    std::vector<TreeNode *> treeNode;   // 导航栏数据模型
    std::vector<ViewNode> viewNodeList; // 用于记录显示的节点
};

#endif
