#ifndef NAV_TREE_VIEW_H
#define NAV_TREE_VIEW_H

/**
 * 树状导航栏控件
 * 1:设置节点数据相当方便,按照对应格式填入即可,分隔符,
 * 2:可设置提示信息 是否显示+宽度
 * 3:可设置行分隔符 是否显示+高度+颜色
 * 4:可设置选中节点线条突出显示+颜色+左侧右侧位置
 * 5:可设置选中节点三角形突出显示+颜色+左侧右侧位置
 * 6:可设置父节点的 选中颜色+悬停颜色+默认颜色
 * 7:可设置子节点的 选中颜色+悬停颜色+默认颜色
 * 8:可设置父节点文字的 图标边距+左侧距离+字体大小+高度
 * 9:可设置子节点文字的 图标边距+左侧距离+字体大小+高度
 * 10:可设置节点展开模式 单击+双击+禁用
 */
#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QListView>
#include <QStringList>
#include <QPainter>

#include "nav_tree_model.h"

class NavTreeView;

class NavTreeDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    NavTreeDelegate(QObject *parent);
    ~NavTreeDelegate();

protected:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    NavTreeView *nav;
};

class NavTreeView : public QListView {
    Q_OBJECT
    Q_ENUMS(ExpendMode)

public:
    //节点展开模式
    enum CollapseMode {
        CollapseMode_SingleClick = 0,  // 单击模式
        CollapseMode_DoubleClick = 1,  // 双击模式
        CollapseMode_NoClick = 2,      // 不可单击双击
    };

    NavTreeView(QWidget *parent = nullptr);
    ~NavTreeView();

private:
    NavTreeModel *model;           // 数据模型
    NavTreeDelegate *delegate;     // 数据委托
    QStringList parentItem;        // 父节点数据集合
    QList<QStringList> childItem;  // 子节点数据

    QString items;              // 节点集合
    CollapseMode collapseMode;    // 节点展开模式 单击/双击/禁用
    bool rightIconVisible;      // 右侧图标是否显示
    bool tipVisible;            // 是否显示提示信息
    int tipWidth;               // 提示信息宽度

    bool separateVisible;  // 是否显示行分隔符
    int separateHeight;    // 行分隔符高度
    QColor separateColor;  // 行分隔符颜色

    bool triangleLeft;     // 是否显示在左侧
    bool triangleVisible;  // 是否显示三角形
    int triangleWidth;     // 三角形宽度
    QColor triangleColor;  // 三角形颜色

    int parentIconMargin;            // 父节点图标边距
    int parentMargin;                // 父节点边距
    int parentFontSize;              // 父节点字体大小
    int parentHeight;                // 父节点高度
    QColor parentBgNormalColor;      // 父节点正常背景色
    QColor parentBgSelectedColor;    // 父节点选中背景色
    QColor parentBgHoverColor;       // 父节点悬停背景色
    QColor parentTextNormalColor;    // 父节点正常文字颜色
    QColor parentTextSelectedColor;  // 父节点选中文字颜色
    QColor parentTextHoverColor;     // 父节点悬停文字颜色

    int childIconMargin;            // 子节点图标边距
    int childMargin;                // 子节点边距
    int childFontSize;              // 子节点字体大小
    int childHeight;                // 子节点高度
    QColor childBgNormalColor;      // 子节点正常背景色
    QColor childBgSelectedColor;    // 子节点选中背景色
    QColor childBgHoverColor;       // 子节点悬停背景色
    QColor childTextNormalColor;    // 子节点正常文字颜色
    QColor childTextSelectedColor;  // 子节点选中文字颜色
    QColor childTextHoverColor;     // 子节点悬停文字颜色

public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setData(const QString path);
    void setData(const QStringList &listItems);

    CollapseMode getCollapseMode() const
    {
        return collapseMode;
    }

    QString getItems() const
    {
        return items;
    }
    bool getRightIconVisible() const
    {
        return rightIconVisible;
    }
    bool getTipVisible() const
    {
        return tipVisible;
    }
    int getTipWidth() const
    {
        return tipWidth;
    }

    bool getSeparateVisible() const
    {
        return separateVisible;
    }
    int getSeparateHeight() const
    {
        return separateHeight;
    }
    QColor getSeparateColor() const
    {
        return separateColor;
    }

    bool getTriangleLeft() const
    {
        return triangleLeft;
    }
    bool getTriangleVisible() const
    {
        return triangleVisible;
    }
    int getTriangleWidth() const
    {
        return triangleWidth;
    }
    QColor getTriangleColor() const
    {
        return triangleColor;
    }

    int getParentIconMargin() const
    {
        return parentIconMargin;
    }
    int getParentMargin() const
    {
        return parentMargin;
    }
    int getParentFontSize() const
    {
        return parentFontSize;
    }
    int getParentHeight() const
    {
        return parentHeight;
    }
    QColor getParentBgNormalColor() const
    {
        return parentBgNormalColor;
    }
    QColor getParentBgSelectedColor() const
    {
        return parentBgSelectedColor;
    }
    QColor getParentBgHoverColor() const
    {
        return parentBgHoverColor;
    }
    QColor getParentTextNormalColor() const
    {
        return parentTextNormalColor;
    }
    QColor getParentTextSelectedColor() const
    {
        return parentTextSelectedColor;
    }
    QColor getParentTextHoverColor() const
    {
        return parentTextHoverColor;
    }

    int getChildIconMargin() const
    {
        return childIconMargin;
    }
    int getChildMargin() const
    {
        return childMargin;
    }
    int getChildFontSize() const
    {
        return childFontSize;
    }
    int getChildHeight() const
    {
        return childHeight;
    }
    QColor getChildBgNormalColor() const
    {
        return childBgNormalColor;
    }
    QColor getChildBgSelectedColor() const
    {
        return childBgSelectedColor;
    }
    QColor getChildBgHoverColor() const
    {
        return childBgHoverColor;
    }
    QColor getChildTextNormalColor() const
    {
        return childTextNormalColor;
    }
    QColor getChildTextSelectedColor() const
    {
        return childTextSelectedColor;
    }
    QColor getChildTextHoverColor() const
    {
        return childTextHoverColor;
    }

public Q_SLOTS:

    //设置节点数据
    // void setItems(const QString &items);

    //设置节点展开模式
    void setCollapseMode(const CollapseMode &collapseMode);

    //设置父节点右侧图标是否显示
    void setRightIconVisible(bool rightIconVisible);

    //设置提示信息 是否显示+宽度
    void setTipVisible(bool tipVisible);
    void setTipWidth(int tipWidth);

    //设置行分隔符 是否显示+高度+颜色
    void setSeparateVisible(bool visible);
    void setSeparateStyle(int height, const QColor &separateColor);

    //设置三角形 位置+可见+宽度+颜色
    void setTriangleVisible(bool triangleVisible);
    void setTriangleStyle(bool triangleLeft, int triangleWidth, const QColor &triangleColor);

    //设置父节点 图标边距+左侧边距+字体大小+节点高度+颜色集合
    void setParentStyle(int parentIconMargin, int parentMargin, int parentHeight, int parentFontSize);
    void setParentBgColor(const QColor &parentBgNormalColor, const QColor &parentBgSelectedColor,
                          const QColor &parentBgHoverColor);
    void setParentTextColor(const QColor &parentTextNormalColor, const QColor &parentTextSelectedColor,
                            const QColor &parentTextHoverColor);

    //设置子节点 图标边距+左侧边距+字体大小+节点高度+颜色集合
    void setChildStyle(int childIconMargin, int childMargin, int childHeight, int childFontSize);
    void setChildBgColor(const QColor &childBgNormalColor, const QColor &childBgSelectedColor,
                         const QColor &childBgHoverColor);
    void setChildTextColor(const QColor &childTextNormalColor, const QColor &childTextSelectedColor,
                           const QColor &childTextHoverColor);

Q_SIGNALS:
    void pressed(int itemIndex);
};

#endif  // NAV_TREE_VIEW_H
