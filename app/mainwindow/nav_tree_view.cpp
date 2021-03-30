/*
 * @Copyright: Copyright (C) 2021-2021 Kevin group. All rights reserved.
 * @Description: file content
 * @Author: Kevin
 * @Email: weikaiup@163.com
 * @Date: 2021-03-18
 */
#include "nav_tree_view.h"
#include <QPainter>
#include "lib/logger/logger.h"
#include "../common/public/pub_env.h"

using namespace KKpub;

NavTreeDelegate::NavTreeDelegate(QObject *parent)
{
    nav = (NavTreeView *)parent;
}

NavTreeDelegate::~NavTreeDelegate()
{
    LogDebug("{} exit", __FUNCTION__);
}

QSize NavTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NavTreeModel::TreeNode *node = (NavTreeModel::TreeNode *)index.data(Qt::UserRole).toULongLong();

    if (node->level == 1) {
        return QSize(50, 35);
    } else {
        return QSize(50, 28);
    }
}

void NavTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    NavTreeModel::TreeNode *node = (NavTreeModel::TreeNode *)index.data(Qt::UserRole).toULongLong();

    //定义变量存储区域
    QRect optionRect = option.rect;
    int x = optionRect.x();
    int y = optionRect.y();
    int width = optionRect.width();
    int height = optionRect.height();
    int fontSize = nav->getParentFontSize();

    //父节点和子节点颜色分开设置
    bool parent = (node->level == 1);

    //根据不同的状态设置不同的颜色 bgColor-主背景色 textColor-主文字颜色 tipBgColor-提示信息背景颜色
    // tipTextColor-提示信息文字颜色
    QColor bgColor, textColor, tipBgColor, tipTextColor, iconColor;
    if (option.state & QStyle::State_Selected) {
        bgColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        textColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipBgColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipTextColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        iconColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
    } else if (option.state & QStyle::State_MouseOver) {
        bgColor = parent ? nav->getParentBgHoverColor() : nav->getChildBgHoverColor();
        textColor = parent ? nav->getParentTextHoverColor() : nav->getChildTextHoverColor();
        tipBgColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        tipTextColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        iconColor = parent ? nav->getParentTextHoverColor() : nav->getChildTextHoverColor();
    } else {
        bgColor = parent ? nav->getParentBgNormalColor() : nav->getChildBgNormalColor();
        textColor = parent ? nav->getParentTextNormalColor() : nav->getChildTextNormalColor();
        tipBgColor = parent ? nav->getParentBgSelectedColor() : nav->getChildBgSelectedColor();
        tipTextColor = parent ? nav->getParentTextSelectedColor() : nav->getChildTextSelectedColor();
        iconColor = parent ? nav->getParentTextNormalColor() : nav->getChildTextNormalColor();
    }

    //绘制背景
    painter->fillRect(optionRect, bgColor);

    // 在选择行绘制三角形，标记选择，目前限定子节点绘制
    int triangleWidth = nav->getTriangleWidth();
    if (!parent && nav->getTriangleVisible() && triangleWidth > 0) {
        if ((option.state & QStyle::State_Selected) || (option.state & QStyle::State_MouseOver)) {
            QFont font = PubEnv::getInstance()->getIconFont();
            font.setPixelSize(fontSize + triangleWidth);
            painter->setFont(font);
            painter->setPen(nav->getTriangleColor());

            //采用图形字体中的三角形绘制
            if (nav->getTriangleLeft()) {
                painter->drawText(optionRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf0da));
            } else {
                painter->drawText(optionRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf0d9));
            }
        }
    }

    //绘制行分隔符
    if (nav->getSeparateVisible()) {
        if (node->level == 1 || (node->level == 2 && node->last)) {
            painter->setPen(QPen(nav->getSeparateColor(), nav->getSeparateHeight()));
            painter->drawLine(QPointF(x, y + height), QPointF(x + width, y + height));
        }
    }

    //绘制文字,如果文字为空则不绘制
    QString text = node->label;
    if (!text.isEmpty()) {
        //文字离左边的距离+字体大小
        int margin = nav->getParentMargin();
        if (node->level == 2) {
            margin = nav->getChildMargin();
            fontSize = nav->getChildFontSize();
        }

        //计算文字区域
        QRect textRect = optionRect;
        textRect.setWidth(width - margin);
        textRect.setX(x + margin);

        QFont font;
        font.setPixelSize(fontSize);
        painter->setFont(font);
        painter->setPen(textColor);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    //绘制提示信息,如果不需要显示提示信息或者提示信息为空则不绘制
    QString tip = node->tip;
    if (nav->getTipVisible() && !tip.isEmpty()) {
        //如果是数字则将超过999的数字显示成 999+
        //如果显示的提示信息长度过长则将多余显示成省略号 .
        if (tip.toInt() > 0) {
            tip = tip.toInt() > 999 ? "999+" : tip;
        } else if (tip.length() > 2) {
            tip = tip.left(2) + " .";
        }

        //计算绘制区域,半径取高度的四分之一
        int radius = height / 4;
        QRect tipRect = optionRect;
        tipRect.setHeight(radius * 2);
        tipRect.moveCenter(optionRect.center());
        tipRect.setLeft(optionRect.right() - nav->getTipWidth() - 5);
        tipRect.setRight(optionRect.right() - 5);

        //设置字体大小
        QFont font;
        font.setPixelSize(fontSize - 2);
        painter->setFont(font);

        //绘制提示文字的背景
        painter->setPen(Qt::NoPen);
        painter->setBrush(tipBgColor);
        painter->drawRoundedRect(tipRect, radius, radius);

        //绘制提示文字
        painter->setPen(tipTextColor);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(tipRect, Qt::AlignCenter, tip);
    }


    if (node->children.size() != 0) {
        //计算绘制图标区域
        QRect iconRect = optionRect;
        iconRect.setLeft(parent ? nav->getParentIconMargin() : nav->getChildIconMargin());

        //设置图形字体和画笔颜色
        QFont font = PubEnv::getInstance()->getIconFont();
        font.setPixelSize(fontSize);
        painter->setFont(font);
        painter->setPen(textColor);

        //绘制左侧图标,有图标则绘制图标,没有的话父窗体取 + -
        if (!node->icon.isNull()) {
            painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, node->icon);
        } else if (parent) {
            if (node->collapse) {
                painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf067));
            } else {
                painter->drawText(iconRect, Qt::AlignLeft | Qt::AlignVCenter, QChar(0xf068));
            }
        }

        //绘制父节点右侧图标
        if (parent && !(nav->getTipVisible() && !node->tip.isEmpty()) && nav->getRightIconVisible()) {
            iconRect.setRight(optionRect.width() - 10);
            if (node->collapse) {
                painter->drawText(iconRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf054));
            } else {
                painter->drawText(iconRect, Qt::AlignRight | Qt::AlignVCenter, QChar(0xf078));
            }
        }
    }
}

NavTreeView::NavTreeView(QWidget *parent) : QListView(parent)
{
    setCollapseMode(CollapseMode_SingleClick);
    setRightIconVisible(true);

    setParentStyle(5, 25, 40, 16);
    setChildStyle(25, 45, 40, 16);
    setParentBgColor(QColor(42, 42, 42), QColor(42, 128, 185), QColor(117, 117, 117));
    setParentTextColor(QColor(126, 127, 134), QColor(211, 231, 240), QColor(255, 255, 255));
    setChildBgColor(QColor(42, 42, 42), QColor(42, 128, 185), QColor(117, 117, 117));
    setChildTextColor(QColor(126, 127, 134), QColor(211, 231, 240), QColor(255, 255, 255));

    setSeparateStyle(2, QColor(126, 127, 134));
    setSeparateVisible(true);
    setTriangleStyle(true, 16, QColor(126, 127, 134));
    setTriangleVisible(false);

    setTipWidth(48);
    setTipVisible(false);

    this->setMouseTracking(true);
    model = new NavTreeModel(this);
    delegate = new NavTreeDelegate(this);

    // 父节点展开方式绑定
    switch (getCollapseMode()) {
        case CollapseMode_SingleClick:
            connect(this, SIGNAL(clicked(QModelIndex)), model, SLOT(collapse(QModelIndex)));
            break;
        case CollapseMode_DoubleClick:
            connect(this, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(collapse(QModelIndex)));
            break;
        default:
            break;
    }
}

NavTreeView::~NavTreeView()
{
    delete model;
    delete delegate;
    LogDebug("{} exit", __FUNCTION__);
}

QSize NavTreeView::sizeHint() const
{
    return QSize(200, 800);
}

QSize NavTreeView::minimumSizeHint() const
{
    return QSize(200, 200);
}

void NavTreeView::setData(const QString xmlPath)
{
    LogDebug("start read xml file >{0}", xmlPath.toStdString());
    model->readDataFromFile(xmlPath);
    this->setModel(model);
    this->setItemDelegate(delegate);

    this->setStyleSheet("background-color: rgb(42, 42, 42);");
}

void NavTreeView::setData(const QStringList &listItem)
{
    model->setData(listItem);
    this->setModel(model);
    this->setItemDelegate(delegate);
}

void NavTreeView::setCollapseMode(const CollapseMode &collapseMode)
{
    this->collapseMode = collapseMode;
}

void NavTreeView::setRightIconVisible(bool rightIconVisible)
{
    this->rightIconVisible = rightIconVisible;
}

void NavTreeView::setTipVisible(bool tipVisible)
{
    this->tipVisible = tipVisible;
}

void NavTreeView::setTipWidth(int tipWidth)
{
    this->tipWidth = tipWidth;
}

void NavTreeView::setSeparateVisible(bool visible)
{
    this->separateVisible = visible;
}

void NavTreeView::setSeparateStyle(int height, const QColor &separateColor)
{
    this->separateHeight = height;
    this->separateColor = separateColor;
}

void NavTreeView::setTriangleVisible(bool triangleVisible)
{
    this->triangleVisible = triangleVisible;
}

void NavTreeView::setTriangleStyle(bool triangleLeft, int triangleWidth, const QColor &triangleColor)
{
    this->triangleLeft = triangleLeft;
    this->triangleWidth = triangleWidth;
    this->triangleColor = triangleColor;
}

void NavTreeView::setParentStyle(int parentIconMargin, int parentMargin, int parentHeight, int parentFontSize)
{
    this->parentIconMargin = parentIconMargin;
    this->parentMargin = parentMargin;
    this->parentHeight = parentHeight;
    this->parentFontSize = parentFontSize;
}

void NavTreeView::setParentBgColor(const QColor &parentBgNormalColor, const QColor &parentBgSelectedColor,
    const QColor &parentBgHoverColor)
{
    if (this->parentBgNormalColor != parentBgNormalColor) {
        this->parentBgNormalColor = parentBgNormalColor;
    }

    if (this->parentBgSelectedColor != parentBgSelectedColor) {
        this->parentBgSelectedColor = parentBgSelectedColor;
    }

    if (this->parentBgHoverColor != parentBgHoverColor) {
        this->parentBgHoverColor = parentBgHoverColor;
    }
}

void NavTreeView::setParentTextColor(const QColor &parentTextNormalColor, const QColor &parentTextSelectedColor,
    const QColor &parentTextHoverColor)
{
    if (this->parentTextNormalColor != parentTextNormalColor) {
        this->parentTextNormalColor = parentTextNormalColor;
    }

    if (this->parentTextSelectedColor != parentTextSelectedColor) {
        this->parentTextSelectedColor = parentTextSelectedColor;
    }

    if (this->parentTextHoverColor != parentTextHoverColor) {
        this->parentTextHoverColor = parentTextHoverColor;
    }
}

void NavTreeView::setChildStyle(int childIconMargin, int childMargin, int childHeight, int childFontSize)
{
    this->childIconMargin = childIconMargin;
    this->childMargin = childMargin;
    this->childHeight = childHeight;
    this->childFontSize = childFontSize;
}

void NavTreeView::setChildBgColor(const QColor &childBgNormalColor, const QColor &childBgSelectedColor,
    const QColor &childBgHoverColor)
{
    if (this->childBgNormalColor != childBgNormalColor) {
        this->childBgNormalColor = childBgNormalColor;
    }

    if (this->childBgSelectedColor != childBgSelectedColor) {
        this->childBgSelectedColor = childBgSelectedColor;
    }

    if (this->childBgHoverColor != childBgHoverColor) {
        this->childBgHoverColor = childBgHoverColor;
    }
}

void NavTreeView::setChildTextColor(const QColor &childTextNormalColor, const QColor &childTextSelectedColor,
    const QColor &childTextHoverColor)
{
    if (this->childTextNormalColor != childTextNormalColor) {
        this->childTextNormalColor = childTextNormalColor;
    }

    if (this->childTextSelectedColor != childTextSelectedColor) {
        this->childTextSelectedColor = childTextSelectedColor;
    }

    if (this->childTextHoverColor != childTextHoverColor) {
        this->childTextHoverColor = childTextHoverColor;
    }
}
