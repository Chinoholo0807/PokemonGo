#ifndef HALLWIDGET_H
#define HALLWIDGET_H

#include <QWidget>

namespace Ui {
class HallWidget;
}

class HallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HallWidget(QWidget *parent = nullptr);//构造函数
    ~HallWidget();//析构函数
    void paintEvent(QPaintEvent *event) override;//重写paintEvent画背景
signals:
    void openPack();//打开背包信号
    void enterFight();//进入战斗大厅信号
    void checkUserList();//检查在线用户信号
    void quit();//退出游戏信号
private:
    Ui::HallWidget *ui;//ui界面指针
};

#endif // HALLWIDGET_H
