/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-19
 */
#include "nav_tree_model.h"

#include "qfile.h"
#include "qdom.h"
#include "qpainterpath.h"
#include "lib/logger/logger.h"

NavTreeModel::~NavTreeModel()
{
    for (std::vector<TreeNode *>::iterator it = treeNode.begin(); it != treeNode.end();) {
        for (std::list<TreeNode *>::iterator child = (*it)->children.begin(); child != (*it)->children.end();) {
            delete (*child);
            child = (*it)->children.erase(child);
        }

        delete (*it);
        it = treeNode.erase(it);
    }
    LogDebug("{} exit", __FUNCTION__);
}

void NavTreeModel::readDataFromFile(const QString path)
{
    QFile xml(path);
    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LogError("Open xml file fail! >{0}", path.toStdString());
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&xml, false)) {
        LogError("Xml to doc fail! >{0}", path.toStdString());
        return;
    }

    treeNode.clear();
    viewNodeList.clear();

    QDomNode root = doc.documentElement().firstChildElement("layout");
    QDomNodeList children = root.childNodes();

    for (int i = 0; i != children.count(); ++i) {
        QDomElement nodeInfo = children.at(i).toElement();
        TreeNode *node = new TreeNode;
        node->label = nodeInfo.attribute("label");
        node->collapse = nodeInfo.attribute("collapse").toInt();
        node->tip = nodeInfo.attribute("tip");
        node->level = 1;

        QDomNodeList secondLevel = nodeInfo.childNodes();

        for (int j = 0; j != secondLevel.count(); ++j) {
            QDomElement secNodeInfo = secondLevel.at(j).toElement();
            TreeNode *secNode = new TreeNode;
            secNode->label = secNodeInfo.attribute("label");
            secNode->tip = secNodeInfo.attribute("tip");
            secNode->collapse = false;
            secNode->level = 2;
            secNode->last = (j == secondLevel.count() - 1 && i != children.count() - 1);
            node->children.push_back(secNode);
        }

        treeNode.push_back(node);
    }
    viewNodeList.push_back(ViewNode({"abc", treeNode[0]}));

    refreshList();
    beginResetModel();
    endResetModel();
}

void NavTreeModel::setData(const QStringList &listItems)
{
    if (listItems.count() == 0) {
        LogError("listItems is empty!");
        return;
    }

    treeNode.clear();
    viewNodeList.clear();

    // listItem格式: 标题|父节点标题(父节点为空)|是否展开|提示信息
    for (int i = 0; i < listItems.count(); i++) {
        QString item = listItems.at(i);
        QStringList list = item.split("|");

        if (list.count() < 4) {
            continue;
        }

        //首先先将父节点即父节点标题为空的元素加载完毕
        QString label = list.at(0);
        QString parentLabel = list.at(1);
        QString collapse = list.at(2);
        QString tip = list.at(3);

        if (parentLabel.isEmpty()) {
            TreeNode *node = new TreeNode;
            node->label = label;
            node->collapse = collapse.toInt();
            node->tip = tip;
            node->level = 1;

            //查找该父节点是否有对应子节点,有则加载
            for (int j = 0; j < listItems.count(); j++) {
                QString secItem = listItems.at(j);
                QStringList secList = secItem.split("|");

                if (secList.count() < 4) {
                    continue;
                }

                QString secTitle = secList.at(0);
                QString secFatherTitle = secList.at(1);
                QString secInfo = secList.at(3);

                if (secFatherTitle == label) {
                    TreeNode *secNode = new TreeNode;
                    secNode->label = secTitle;
                    secNode->tip = secInfo;
                    secNode->collapse = false;
                    secNode->level = 2;
                    secNode->last = (j == listItems.count() - 1);
                    node->children.push_back(secNode);
                }
            }

            treeNode.push_back(node);
        }
    }

    refreshList();
    beginResetModel();
    endResetModel();
}

int NavTreeModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(viewNodeList.size());
}

QVariant NavTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    // index 不在显示节点列表的范围 viewNodeList
    if (index.row() >= static_cast<int>(viewNodeList.size()) || index.row() < 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return viewNodeList[index.row()].label;
    } else if (role == Qt::UserRole) {
        return (uintptr_t)(viewNodeList[index.row()].treeNode);
    }

    return QVariant();
}

void NavTreeModel::refreshList()
{
    viewNodeList.clear();

    for (std::vector<TreeNode *>::iterator it = treeNode.begin(); it != treeNode.end(); ++it) {
        ViewNode node;
        node.label = (*it)->label;
        node.treeNode = *it;

        viewNodeList.push_back(node);

        if ((*it)->collapse) {
            continue;
        }

        for (std::list<TreeNode *>::iterator child = (*it)->children.begin(); child != (*it)->children.end(); ++child) {
            ViewNode node;
            node.label = (*child)->label;
            node.treeNode = *child;
            node.treeNode->last = false;
            viewNodeList.push_back(node);
        }

        if (!viewNodeList.empty()) {
            viewNodeList.back().treeNode->last = true;
        }
    }
}

void NavTreeModel::collapse(const QModelIndex &index)
{
    TreeNode *node = viewNodeList[index.row()].treeNode;

    LogDebug("You clicked the item: {}", node->label.toStdString());
    if (node->children.size() == 0) {
        return;
    }

    // 取反折叠标志，折叠则删除子节点，展开则增加子节点
    node->collapse = !node->collapse;
    if (!node->collapse) {
        beginInsertRows(QModelIndex(), index.row() + 1, index.row() + static_cast<int>(node->children.size()));
        endInsertRows();
    } else {
        beginRemoveRows(QModelIndex(), index.row() + 1, index.row() + static_cast<int>(node->children.size()));
        endRemoveRows();
    }
    // 重新刷新显示节点数据
    refreshList();
}
