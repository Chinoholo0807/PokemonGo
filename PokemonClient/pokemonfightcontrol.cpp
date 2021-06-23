#include "pokemonfightcontrol.h"
#include<QThread>
#include<QTimer>
#include <synchapi.h>
PokemonFightControl::PokemonFightControl(QObject* p):
    QObject(p)
{

}
void PokemonFightControl::startFight(Pokemon* pokemonA,Pokemon* pokemonB){//开启一场战斗
    srand(time(NULL));//设置一场战斗中的随机数
    this->A=pokemonA;
    this->B=pokemonB;
    round=0;//回合数清0
    gameIsEnd=false;//战斗进行中
    A->readyFight();
    B->readyFight();
    A->printState();
    B->printState();
}
void PokemonFightControl::useSkill(int idxA,int idxB){//使用技能，根据速度决定出手顺序，idxA为A使用技能编号，idxB为B使用技能编号
    int idxFirst,idxSecond;
    //根据速度决定出手顺序，若相等则随机
    if(A->getSpeed()>B->getSpeed()){//A的速度大于B，本回合A先手
        first=A;idxFirst=idxA;
        second=B;idxSecond=idxB;
    }else if(B->getSpeed()>A->getSpeed()){//A的速度小于B，本回合B先手
        first=B;idxFirst=idxB;
        second=A;idxSecond=idxA;
    }else{//速度相等
        if(rand()%2){
            first=A;idxFirst=idxA;
            second=B;idxSecond=idxB;
        }else{
            first=B;idxFirst=idxB;
            second=A;idxSecond=idxA;
        }
    }

    first->resetFlag();
    second->resetFlag();
    first->skill(idxFirst,second);//先手精灵使用技能
    //发出信号来处理相应界面显示（动画、血条变动等)
    //若为伤害型技能，会先判定释放命中，是否暴击，若为效果型技能，则默认普通命中动画
    if(first->getHitFlag()){//命中，显示攻击命中动画
        if(first->getCritFlag()){//暴击，显示攻击暴击动画
            if(first==A)
                emit  skillDetermine(true,true,true);
            else
                emit  skillDetermine(false,true,true);
        }else{//只显示攻击命中动画
            if(first==A)
               emit  skillDetermine(true,false,true);
            else
               emit skillDetermine(false,true,true);
        }
    }else{//攻击未命中，显示攻击闪避动画
        if(first==A)
            emit skillDetermine(true,false,false);
        else
            emit skillDetermine(false,false,false);
    }

    //接下来判定后手小精灵是否受到伤害
    if(second->getHurtFlag()){//受到伤害,变动血条,同时显示受击动画
        if(second==A){
            emit hurtDetermine(true,second->getHurtFlag());
        }
        else
            emit hurtDetermine(false,second->getHurtFlag());

    }

    if(!second->isDead()){//后手小精灵未死亡
        second->skill(idxSecond,first);//后手小精灵使用技能
        if(second->getHitFlag()){//命中，显示攻击命中动画
            if(second->getCritFlag()){//暴击，显示攻击暴击动画
                if(second==A)
                    emit skillDetermine(true,true,true);
                else
                    emit skillDetermine(false,true,true);
            }else{//只显示攻击命中动画
                if(second==A)
                    emit skillDetermine(true,false,true);
                else
                    emit skillDetermine(false,false,true);
            }
        }else{//攻击未命中，显示攻击闪避动画
            if(second==A)
                emit skillDetermine(true,false,false);
            else
                emit skillDetermine(false,false,false);
        }

        //接下来判定先手小精灵是否受到伤害
        if(first->getHurtFlag()){//受到伤害,变动血条,同时显示受击动画
            if(first==A)
                emit hurtDetermine(true,first->getHurtFlag());
            else
                emit hurtDetermine(false,first->getHurtFlag());
        }
    }
    round++;//回合数加1
    A->printState();//显示A的状态
    B->printState();//显示B的状态
    if(A->isDead()){
        gameIsEnd=true;
        qDebug()<<"Pokemon "<<A->getName()<<" is dead.Game Over.";

    }
    if(B->isDead()){
        gameIsEnd=true;
        qDebug()<<"Pokemon "<<B->getName()<<" is dead.Game Over.";

    }

}
