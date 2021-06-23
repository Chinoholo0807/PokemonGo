#ifndef PACKWIDGET_H
#define PACKWIDGET_H

#include <QWidget>
#include"userinfo.h"
#include"pokemon.h"
namespace Ui {
class PackWidget;
}

class PackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PackWidget(QWidget *parent = nullptr);//构造函数
    ~PackWidget();//析构函数
    void reflesh();//刷新显示
    void setUserInfo( UserInfo thatInfo,QList<Pokemon*> list)//设置用户信息
    {this->info=thatInfo;this->userPokemonList=list;}
    void paintEvent(QPaintEvent *event) override;//重写paintEvent
signals:
    void back();//返回大厅信号
private:
    Ui::PackWidget *ui;//ui界面指针
    UserInfo info;//用户信息
    QList<Pokemon* > userPokemonList;//用户小精灵信息
};

#endif // PACKWIDGET_H
