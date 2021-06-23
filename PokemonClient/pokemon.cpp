#include "pokemon.h"
#include<QDebug>
//构造函数
Pokemon::Pokemon(QString name1,PokemonType type1,int level1,int exp1,int uid1,Attribute attribute):
    name(name1),type(type1),level(level1),exp(exp1),uid(uid1),defaultAttribute(attribute),currentAttribute(attribute){
    exp=0;
    if(level1==1){
        tryLevelUp(exp1);
        return ;
    }
    int allExp=exp1;
    if(level>=2&&level<=15){
        while(level>=2){
            allExp+=LEVEL_UP_EXP[level-1];
            level--;
        }
        tryLevelUp(allExp);//模拟升级
    }
}
//显示状态，调试用
void Pokemon::printState() const{
    QString state;
    state+=QString("name: %1 type %2 level:%3 uid:%4 ")\
            .arg(name).arg(type).arg(level).arg(uid);
    state+=QString("atk:%1 def:%2 speed:%3 [HP:%4] crit:%5 dodge:%6 ")\
            .arg(getAttack()).arg(getDefinse()).arg(getSpeed()).arg(getHP()).arg(getCrit()).arg(getDodge());
    qDebug()<<state;
}
//获得经验尝试升级，升级成功返回true
bool Pokemon::tryLevelUp(int getExp){
    exp+=getExp;
    if(MAX_LEVEL == level){//到达当前最高等级，exp清零
        exp=0;
        return false;
    }
    bool levelUpFlag=false;
    while(LEVEL_UP_EXP[level]<=exp&&level<MAX_LEVEL){
        exp-=LEVEL_UP_EXP[level];
        levelUp();
        levelUpFlag=true;
    }
    return true;
}
//升级,增加属性
void Pokemon::levelUp(){
 //   qDebug()<<name<<" level up! "<<level<<" --->"<<level+1;
    switch (type) {
    case Strength://力量型
        defaultAttribute.attack+=5;
        break;
    case Tank://肉盾型
        defaultAttribute.HP+=10;
        break;
    case Definsive://防御型
        defaultAttribute.definse+=5;
        break;
    case Agile://敏捷型
        defaultAttribute.speed+=5;
        break;
    default:
        break;
    }
    defaultAttribute.attack+=5;
    defaultAttribute.definse+=5;
    defaultAttribute.speed+=5;
    defaultAttribute.HP+=10;
    ++level;
    currentAttribute=defaultAttribute;
}
//造成伤害判定，已经判断过闪避率和暴击率，若暴击，传入伤害为暴击后的伤害
void Pokemon::hunt(int damage, Pokemon *to){
    to->hurt(damage,this);
}
//受到伤害判断，已经判断过闪避率和暴击率，若暴击，传入伤害为暴击后的伤害
void Pokemon::hurt(int damage, Pokemon *from){
    //伤害计算公式
    int l=from->getLevel();
    int a=from->getAttack();
    int d=getDefinse();
    int finalDamage=(l*0.4+2)*damage*a/(d*5)+2;

    this->currentAttribute.HP-=finalDamage;
    hurtFlag=true;//受击
    hurtDamage=finalDamage;//受击伤害
    if(currentAttribute.HP<0){
        currentAttribute.HP=0;
    }
}
//判断一次攻击是否命中
bool Pokemon::isDodge(double dodge){
    int bonus = rand()%101;//0----100
    if(bonus<=(int)(dodge*100))//闪避
        return true;
    else//命中
        return false;
}
//判定一次攻击是否暴击
bool Pokemon::isCrit(double crit){
    int bonus = rand()%101;//0----100
    if(bonus>(int)(crit*100))//没有暴击
        return false;
    else//暴击,双倍伤害
        return true;
}

//小火龙
XiaoHuoLong::XiaoHuoLong(int level,int exp):
    //小火龙，力量型
    Pokemon("小火龙",Strength,level,exp,0,Attribute(50,50,100,50)){
    skillName[0]="火花";
    skillName[1]="火焰牙";
    skillName[2]="愤怒";
    skillName[3]="闪焰冲锋";
    skillDescription[0]="威力40";
    skillDescription[1]="威力65";
    skillDescription[2]="攻击提升一个等级";
    skillDescription[3]="威力120";

}
//小火龙技能
void XiaoHuoLong::skill(int skillIdx,Pokemon* to){
    qDebug()<<"XiaoHuoLong ";
    switch(skillIdx){
    case 1://技能2 火焰牙 威力65
        qDebug()<<"Use skill 2";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(130,to);
                critFlag=true;
            }else{
                hunt(65,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    case 2://技能3 愤怒 攻击提升一个等级
        qDebug()<<"Use skill 3";
        critFlag=false;
        hitFlag=false;
        this->currentAttribute.attack*=1.5;
        break;
    case 3://技能4 闪焰冲锋 威力120
        qDebug()<<"Use skill 4";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(240,to);
                critFlag=true;
            }else{
                hunt(120,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    default://技能1 火花 威力40
        qDebug()<<"Use skill 1";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(80,to);
                critFlag=true;
            }else{
                hunt(40,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    }
}

//妙蛙种子
MiaoWaZhongZi::MiaoWaZhongZi(int level,int exp):
    //妙蛙种子，肉盾型
    Pokemon("妙蛙种子",Tank,level,exp,1,Attribute(40,50,120,45)){
    skillName[0]="藤鞭";
    skillName[1]="光合作用";
    skillName[2]="木叶飞刀";
    skillName[3]="阳光烈焰";
    skillDescription[0]="威力45";
    skillDescription[1]="回复最大生命值的1/2";
    skillDescription[2]="必中 威力65";
    skillDescription[3]="威力120";
}
//妙蛙种子技能
void MiaoWaZhongZi::skill(int skillIdx,Pokemon* to){
    qDebug()<<"MiaoWaZhongZi ";
    switch(skillIdx){
    case 1://技能2 光合作用 回复最大生命值的1/2
    {
        qDebug()<<"Use skill 2";
        critFlag=false;
        hitFlag=false;
        int gainHP=this->getDHP()*0.5;
        qDebug()<<gainHP;
        qDebug()<<this->getHP();
        if(this->getHP()+gainHP>=this->getDHP())
            this->currentAttribute.HP=this->getDHP();
        else
            this->currentAttribute.HP+=gainHP;
        break;
    }
    case 2://技能3 木叶飞刀 必中 威力65
        qDebug()<<"Use skill 3";        
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(130,to);
                critFlag=true;
            }else{
                hunt(65,to);
                critFlag=false;
            }
        break;
    case 3://技能4 阳光烈焰 威力120
        qDebug()<<"Use skill 4";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(240,to);
                critFlag=true;
            }else{
                hunt(120,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    default://技能1 藤鞭 威力45
        qDebug()<<"Use skill 1";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(90,to);
                critFlag=true;
            }else{
                hunt(45,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    }
}

//杰尼龟
JieNiGui::JieNiGui(int level,int exp):
    //杰尼龟，防御型
    Pokemon("杰尼龟",Definsive,level,exp,2,Attribute(40,60,100,40)){
    skillName[0]="水枪";
    skillName[1]="水流喷射";
    skillName[2]="铁壁";
    skillName[3]="火箭头锤";
    skillDescription[0]="威力40";
    skillDescription[1]="威力60";
    skillDescription[2]="防御提升一个等级";
    skillDescription[3]="威力130";
}
//杰尼龟技能
void JieNiGui::skill(int skillIdx,Pokemon* to){
    qDebug()<<"JieNiGui ";
    switch(skillIdx){
    case 1://技能2 水流喷射 威力60
        qDebug()<<"Use skill 2";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(120,to);
                critFlag=true;
            }else{
                hunt(60,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    case 2://技能3 铁壁 防御提升一个等级
        qDebug()<<"Use skill 3";
        critFlag=false;
        hitFlag=false;
        this->currentAttribute.definse*=1.5;
        break;
    case 3://技能4 火箭头锤 威力130
        qDebug()<<"Use skill 4";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(260,to);
                critFlag=true;
            }else{
                hunt(130,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    default://技能1 水枪 威力40
        qDebug()<<"Use skill 1";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(80,to);
                critFlag=true;
            }else{
                hunt(40,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    }
}

//皮卡丘
Pikaqiu::Pikaqiu(int level,int exp):
    //皮卡丘,敏捷型
    Pokemon("皮卡丘",Agile,level,exp,3,Attribute(50,50,100,60)){
    skillName[0]="电击";
    skillName[1]="电球";
    skillName[2]="高速移动";
    skillName[3]="十万伏特";
    skillDescription[0]="威力40";
    skillDescription[1]="小精灵速度差越大，威力越大";
    skillDescription[2]="速度提升2个等级";
    skillDescription[3]="必中 威力100";
}
//皮卡丘技能
void Pikaqiu::skill(int skillIdx,Pokemon* to){
    qDebug()<<"Pikaqiu ";
    switch(skillIdx){
    case 1://技能2 电球 小精灵速度差越大，威力越大
        qDebug()<<"Use skill 2";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            double rate=getSpeed()/to->getSpeed();
            int causeDamage;
            //速度差越大，该技能造成威力越大
            if(rate<=1)
                causeDamage=40;
            else if(rate>1&&rate<=2)
                causeDamage=80;
            else if(rate>2&&rate<=3)
                causeDamage=120;
            else
                causeDamage=200;
            if(isCrit(getCrit())){//暴击
                hunt(causeDamage*2,to);
                critFlag=true;
            }else{
                hunt(causeDamage,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    case 2://技能3 高速移动 速度提升2个等级
        qDebug()<<"Use skill 3";
        critFlag=false;
        hitFlag=false;
        this->currentAttribute.speed*=2.25;
        break;
    case 3://技能4 十万伏特 必中 威力100
        qDebug()<<"Use skill 4";      
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(200,to);
                critFlag=true;
            }else{
                hunt(100,to);
                critFlag=false;
            }     
        break;
    default://技能1 电击 威力40
        qDebug()<<"Use skill 1";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(80,to);
                critFlag=true;
            }else{
                hunt(40,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    }
}

//胖丁
Pangding::Pangding(int level,int exp):
    //胖丁,力量型
    Pokemon("胖丁",Strength,level,exp,4,Attribute(60,50,100,50)){
    skillName[0]="拍击";
    skillName[1]="魅惑之声";
    skillName[2]="睡觉";
    skillName[3]="泰山压顶";
    skillDescription[0]="威力40";
    skillDescription[1]="必中 威力60";
    skillDescription[2]="攻击和防御提升1个等级";
    skillDescription[3]="威力120";
}
//胖丁技能
void Pangding::skill(int skillIdx,Pokemon* to){
    qDebug()<<"Pangding ";
    switch(skillIdx){
    case 1://技能2 魅惑之声 必中 威力60
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(120,to);
                critFlag=true;
            }else{
                hunt(60,to);
                critFlag=false;
            }
        break;
    case 2://技能3 睡觉 攻击和防御提升1个等级
        qDebug()<<"Use skill 3";
        hitFlag=false;
        critFlag=false;
        this->currentAttribute.attack*=1.5;
        this->currentAttribute.definse*=1.5;
        break;
    case 3://技能4 泰山压顶 威力120
        qDebug()<<"Use skill 4";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(240,to);
                critFlag=true;
            }else{
                hunt(120,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    default://技能1 拍击 威力40
        qDebug()<<"Use skill 1";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(80,to);
                critFlag=true;
            }else{
                hunt(40,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    }
}

//可达鸭
Kedaya::Kedaya(int level,int exp):
    //可达鸭,肉盾型
    Pokemon("可达鸭",Tank,level,exp,5,Attribute(50,50,130,45)){
    skillName[0]="水枪";
    skillName[1]="水之波动";
    skillName[2]="瞬间失忆";
    skillName[3]="水炮";
    skillDescription[0]="威力40";
    skillDescription[1]="必中 威力60";
    skillDescription[2]="防御提升2个等级";
    skillDescription[3]="威力110";
}
//可达鸭
void Kedaya::skill(int skillIdx,Pokemon* to){
    qDebug()<<"Kedaya ";
    switch(skillIdx){
    case 1://技能2 水之波动 必中 威力60
        qDebug()<<"Use skill 2";      
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(120,to);
                critFlag=true;
            }else{
                hunt(60,to);
                critFlag=false;
            }        
        break;
    case 2://技能3 瞬间失忆 防御提升2个等级
        hitFlag=false;
        critFlag=false;
        this->currentAttribute.definse*=2.25;
        break;
    case 3://技能4 水炮 威力110
        qDebug()<<"Use skill 4";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(220,to);
                critFlag=true;
            }else{
                hunt(110,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    default://技能1 水枪 威力40
        qDebug()<<"Use skill 1";
        if(!isDodge(to->getDodge())){//攻击命中
            hitFlag=true;
            if(isCrit(getCrit())){//暴击
                hunt(80,to);
                critFlag=true;
            }else{
                hunt(40,to);
                critFlag=false;
            }
        }else{
            hitFlag=false;
        }
        break;
    }
}

