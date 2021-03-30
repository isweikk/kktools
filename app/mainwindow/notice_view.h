#ifndef NOTICEMSG_H
#define NOTICEMSG_H

#include <QWidget>
#include <qlabel.h>

class NoticeView : public QWidget {
public:
    NoticeView(QWidget *parent);
    ~NoticeView();

    struct InfoModel {
        QLabel* textLabel;
        QLabel* picLabel;
    };

    void setModel(InfoModel infoModel);

protected:
    void initData();
    void changeNoticeView();

    void mousePressEvent(QMouseEvent* event);

private:
    InfoModel info;

    int picNum = 0;
    QStringList picList;
    QStringList textList;

signals:
    void clicked();
};

#endif // NOTICEMSG_H
