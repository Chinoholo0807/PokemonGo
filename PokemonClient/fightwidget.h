#ifndef FIGHTWIDGET_H
#define FIGHTWIDGET_H

#include <QWidget>
#include"userinfo.h"
#include"pokemon.h"
namespace Ui {
class FightWidget;
}

class FightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FightWidget(QWidget *parent = nullptr);//构造函数
    ~FightWidget();//析构函数
    void randGeneratePInfo();//随机生成敌方小精灵信息
    void reflesh();//刷新显示
    void setUserInfo(UserInfo *thatInfo,QList<Pokemon*> *list)//设置用户信息
    {this->info=thatInfo;this->userPokemonList=list;}
    void paintEvent(QPaintEvent *event) override;//重写paintEvent
signals:
    void back();//返回大厅信号
    void readyFight(int mode0,Pokemon*A,Pokemon*B,int autoFlag);//战斗准备开始信号
private:
    QList<Pokemon* > rivalPokemonList;//敌方小精灵信息列表
    Pokemon* pokemonA;//出战己方小精灵
    Pokemon* pokemonB;//对战敌方小精灵
    Ui::FightWidget *ui;//ui界面指针
    UserInfo *info;//用户信息
    QList<Pokemon* > *userPokemonList;//用户小精灵信息
};

#endif // FIGHTWIDGET_H
