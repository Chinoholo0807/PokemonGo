#ifndef POKEMONFIGHTCONTROL_H
#define POKEMONFIGHTCONTROL_H
#include"pokemon.h"
#include<QDebug>
#include<QObject>
//用于控制一场小精灵之间的战斗的类
class PokemonFightControl:public QObject
{
    Q_OBJECT
public:

    PokemonFightControl(QObject*parent =nullptr);
    ~PokemonFightControl(){};
    bool gameOver()const{return gameIsEnd;}//判断一场战斗是否结束
    int getRound()const{return round;}//返回战斗进行回合数

    void startFight(Pokemon* pokemonA,Pokemon* pokemonB);//开启一场战斗
    void useSkill(int idxA,int idxB);//使用技能，根据速度决定出手顺序，idxA为A使用技能编号，idxB为B使用技能编号
signals:
    void skillDetermine(bool isLeft,bool critFlag,bool hitFlag);//发出使用技能信号，用于动画处理
    void hurtDetermine(bool isLeft,int damage);//发出受击信号，用于动画处理
private:
    int round=0;//总回合数
    bool gameIsEnd=false;//战斗结束标志
    Pokemon *A;//小精灵A
    Pokemon *B;//小精灵B
    Pokemon *first;//某一回合中的先手方
    Pokemon *second;//某一回合中的后手方
};

#endif // POKEMONFIGHTCONTROL_H
