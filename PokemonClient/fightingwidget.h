#ifndef FIGHTINGWIDGET_H
#define FIGHTINGWIDGET_H
#include"pokemon.h"
#include"pokemonfightcontrol.h"
#include"userinfo.h"
#include <QWidget>
#include<QLabel>
#include<QPropertyAnimation>
#include<QSequentialAnimationGroup>
/*-----------游戏模式定义-----------*/
#define UP_LEVEL_GAME  133//升级赛
#define DUEL_GAME      134//决斗赛
/*--------------------------------*/
namespace Ui {
class FightingWidget;
}

class FightingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FightingWidget(QWidget *parent = nullptr);//构造函数
    ~FightingWidget();//析构函数
    void startBattle(Pokemon *pokemonA,Pokemon *pokemonB,int gameMode);//启动一场战斗,gameMode为游戏模式
    void setUserInfo(UserInfo *thatInfo,QList<Pokemon*> *list)//设置用户信息
    {this->info=thatInfo;this->userPokemonList=list;}
    void setFlag(int flag){autoFlag=flag;}//设置自动战斗标志
    void paintEvent(QPaintEvent *event) override;//重写paintEvent
signals:
    void backToFightWidget();//返回战斗大厅信号
    void gameIsOver(Pokemon*pokemonA,Pokemon*pokemonB,int gameMode,int packIdx);//游戏结束信号
public slots:
    void pressSkillButton(int i);//按下技能按钮
    void pressDiscardButton();//按下丢弃精灵按钮
    void dealSkill(bool isLeft ,bool critFlag,bool hitFlag);//处理技能，增加攻击动画
    void dealHurt(bool isLeft,int damage);//处理受击判断，增加受击动画
    void dealDiscardPokemon();//处理丢弃小精灵
private:
    Ui::FightingWidget *ui;//ui界面指针
    int gameMode;//游戏模式
    int idx[3];//丢弃精灵时3个精灵在背包中的位置
    int autoFlag;//自动战斗标志
    Pokemon *pokemonA;//宠物小精灵A
    Pokemon *pokemonB;//宠物小精灵B

    PokemonFightControl *control;//战斗控制的类
    QSequentialAnimationGroup* action;//串行动画
    QLabel *opA;//小精灵A的gif
    QLabel *opB;//小精灵B的gif
    QString stateText;//战斗信息打印String
    UserInfo *info;//用户信息
    QList<Pokemon* > *userPokemonList;//用户小精灵信息
};

#endif // FIGHTINGWIDGET_H
