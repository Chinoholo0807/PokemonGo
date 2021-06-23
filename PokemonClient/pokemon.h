#ifndef POKEMON_H
#define POKEMON_H
#include<QString>

#define MAX_LEVEL 15 //最大等级
//小精灵的类型
enum PokemonType{
    Strength,//力量型:高攻击
    Tank,//肉盾型:高生命值
    Definsive,//防御型:高防御值
    Agile,//敏捷性:低攻击间隔 即高速度
    Special//特殊性:属性自定
};
//小精灵的各种属性
class Attribute{
public:
    Attribute(int attack1,int definse1,int HP1,int speed1,double crit1=0.1,double dodge1=0.1):
        attack(attack1),definse(definse1),HP(HP1),speed(speed1),crit(crit1),dodge(dodge1){}
    Attribute(const Attribute & that){//拷贝构造
        attack=that.attack;
        definse=that.definse;
        HP=that.HP;
        speed=that.speed;
        crit=that.crit;
        dodge=that.dodge;
    }
    int attack;//攻击力
    int definse;//防御力
    int HP;//生命值
    int speed;//速度，攻击间隔的倒数
    double crit;//暴击率
    double dodge;//闪避率
    Attribute & operator =(const Attribute & that){//重载运算符=
        if(this!=&that){
        attack=that.attack;
        definse=that.definse;
        HP=that.HP;
        speed=that.speed;
        crit=that.crit;
        dodge=that.dodge;
        }
        return *this;
    }
};

//小精灵升级所需经验值,LEVEL_UP_EXP[i]代表从i级升到i+1级所需的经验值,最低为1级
const int LEVEL_UP_EXP[MAX_LEVEL]={
  //0  1   2   3   4   5   6   7   8   9  10  11  12  13   14
    0,100,100,100,120,140,160,180,200,250,300,400,500,600,700
};

//小精灵类
class Pokemon
{
public:
    //Pokemon(); 构造函数
    Pokemon(QString name1,PokemonType type1,int level1,int exp1,int uid1,Attribute attribute);
    Pokemon(QString name1,PokemonType type1,int uid1,Attribute attribute):
        Pokemon(name1,type1,1,0,uid1,attribute){}//委托构造
    virtual ~Pokemon()=default;//析构函数

    virtual bool tryLevelUp(int getExp);//获得经验尝试升级，升级成功返回true,否则返回false
    virtual void levelUp();//升级,增加属性
    virtual void skill(int i,Pokemon * to)=0;//小精灵的技能，每个小精灵有4个技能，其中必定有一个普通攻击技能，剩下3个技能升级解锁
    virtual void hurt(int damage,Pokemon * from);//受到伤害时的判定
    virtual void hunt(int damage,Pokemon * to);//造成伤害时的判定
    virtual bool isDodge(double dodge);//判断一次攻击是否命中
    virtual bool isCrit(double crit);//判定一次攻击是否暴击
    bool isDead()const{return currentAttribute.HP==0;};//判断精灵是否已经死亡
    void readyFight(){currentAttribute=defaultAttribute;};//准备战斗,重置类内Flag等
    virtual void printState()const;//显示状态，调试用
    //获取技能及描述
    QString getSkillName(int i){
        if(i>=0&&i<=3)
            return skillName[i];
        return "UNKNOWN";
    }
    QString getSkillDesc(int i){
        if(i>=0&&i<=3)
            return skillDescription[i];
        return "UNKNOWN";
    }
    //获取非战斗属性
    QString getName()const{return name;}
    PokemonType getType()const{return type;}
    int getLevel()const{return level;}
    int getExp()const{return exp;}
    int getUid()const{return uid;}

    //获取defaultAttribute默认属性
    int getDAttack()const{return defaultAttribute.attack;}
    int getDDefinse()const{return defaultAttribute.definse;}
    int getDHP()const{return defaultAttribute.HP;}
    int getDSpeed()const{return defaultAttribute.speed;}
    double getDCrit()const{return defaultAttribute.crit;}
    double getDDodge()const{return defaultAttribute.dodge;}

    //获取currentAttribute战斗属性
    int getAttack()const{return currentAttribute.attack;}
    int getDefinse()const{return currentAttribute.definse;}
    int getHP()const{return currentAttribute.HP;}
    int getSpeed()const{return currentAttribute.speed;}
    double getCrit()const{return currentAttribute.crit;}
    double getDodge()const{return currentAttribute.dodge;}

    //获取Flag
    bool getCritFlag()const{return critFlag;}//获取暴击标志
    bool getHitFlag()const{return hitFlag;}//获取命中标志
    int getHurtFlag()const{//获取受击标志，若受击返回受到伤害
        if(hurtFlag) return hurtDamage;
        return 0;
    }
    void resetFlag(){//重置flag
        critFlag=false;
        hurtFlag=false;
        hitFlag=false;
        hurtDamage=0;
    }
protected:
    QString name;//名字
    PokemonType type;//类型
    int level;//等级
    int exp;//当前等级经验值
    int uid;//小精灵的uid,一个uid代表一个小精灵类
    Attribute defaultAttribute;//默认属性
    Attribute currentAttribute;//当前属性

   //用于扩展各种技能的实现以及界面控制
    bool critFlag;//暴击标志
    bool hitFlag;//命中标志
    bool hurtFlag;//受伤标记
    int hurtDamage;//受到伤害
    //四个技能的名字
    QString skillName[4]={"技能1","技能2","技能3","技能4"};
    //四个技能的描述
    QString skillDescription[4]={"技能1描述","技能2描述","技能3描述","技能4描述"};

};
//小火龙
class XiaoHuoLong :public Pokemon{
public:
    XiaoHuoLong(int level,int exp);
    XiaoHuoLong():XiaoHuoLong(1,0){};//委托构造
    ~XiaoHuoLong()=default;
    void skill(int i,Pokemon * to) override; //重写技能
};
//妙蛙种子
class MiaoWaZhongZi :public Pokemon{
public:
    MiaoWaZhongZi(int level,int exp);
    MiaoWaZhongZi():MiaoWaZhongZi(1,0){};//委托构造
    ~MiaoWaZhongZi()=default;
    void skill(int i,Pokemon * to) override; //重写技能
};
//杰尼龟
class JieNiGui:public Pokemon{
public:
    JieNiGui(int level,int exp);
    JieNiGui():JieNiGui(1,0){};//委托构造
    ~JieNiGui()=default;
    void skill(int i,Pokemon * to) override; //重写技能
};
//皮卡丘
class Pikaqiu:public Pokemon{
public:
    Pikaqiu(int level,int exp);
    Pikaqiu():Pikaqiu(1,0){};//委托构造
    ~Pikaqiu()=default;
    void skill(int i,Pokemon * to) override; //重写技能
};
//胖丁
class Pangding:public Pokemon{
public:
    Pangding(int level,int exp);
    Pangding():Pangding(1,0){};//委托构造
    ~Pangding()=default;
    void skill(int i,Pokemon * to) override; //重写技能
};
//可达鸭
class Kedaya:public Pokemon{
public:
    Kedaya(int level,int exp);
    Kedaya():Kedaya(1,0){};//委托构造
    ~Kedaya()=default;
    void skill(int i,Pokemon * to) override; //重写技能
};

//用于生成小精灵的类
class PokemonGenerate{
public:
    static const int PNum=6;//当前小精灵种类数
    //生成小精灵的工厂，使用完记得delete
    static Pokemon* generatePokemon(int uid,int level,int exp){
        switch (uid) {
        case 0:
            return new XiaoHuoLong(level,exp);
            break;
        case 1:
            return new MiaoWaZhongZi(level,exp);
            break;
        case 2:
            return new JieNiGui(level,exp);
            break;
        case 3:
            return new Pikaqiu(level,exp);
            break;
        case 4:
            return new Pangding(level,exp);
            break;
        case 5:
            return new Kedaya(level,exp);
            break;
        default:
            return nullptr;
            break;
        }
    }
    //存放uid对应小精灵的名字
    static QString name(int uid){
        switch (uid) {
        case 0:
            return "小火龙";
        case 1:
            return "妙蛙种子";
        case 2:
            return "杰尼龟";
        case 3:
            return "皮卡丘";
        case 4:
            return "胖丁";
        case 5:
            return "可达鸭";
        default:
            return "UNKNOWN";
        }
    }
    //存放uid对应小精灵的图片
    static QString picPath(int uid){
        switch (uid) {
        case 0:
            return ":/res/pokemonPic/charmander.png";
        case 1:
            return ":/res/pokemonPic/bulbasaur.png";
        case 2:
            return ":/res/pokemonPic/squirtle.png";
        case 3:
            return ":/res/pokemonPic/pikachu.png";
        case 4:
            return ":/res/pokemonPic/jigglypuff.png";
        case 5:
            return ":/res/pokemonPic/psyduck.png";
        default:
            return "UNKNOWN";
        }
    }
    //存储uid对应小精灵的gif，战斗中显示
    static QString gifPath(int uid,int isLeft){
        switch (uid) {
        case 0:
            if(isLeft)
                return ":/res/pokemonPic/charmanderL.gif";
            else
                return ":/res/pokemonPic/charmanderR.gif";
            break;
        case 1:
            if(isLeft)
                return ":/res/pokemonPic/bulbasaurL.gif";
            else
                return ":/res/pokemonPic/bulbasaurR.gif";
            break;
        case 2:
            if(isLeft)
                return ":/res/pokemonPic/squirtleL.gif";
            else
                return ":/res/pokemonPic/squirtleR.gif";
            break;
        case 3:
            if(isLeft)
                return ":/res/pokemonPic/pikachuL.gif";
            else
                return ":/res/pokemonPic/pikachuR.gif";
            break;
        case 4:
            if(isLeft)
                return ":/res/pokemonPic/jigglypuffL.gif";
            else
                return ":/res/pokemonPic/jigglypuffR.gif";
            break;
        case 5:
            if(isLeft)
                return ":/res/pokemonPic/psyduckL.gif";
            else
                return ":/res/pokemonPic/psyduckR.gif";
            break;
        default:
            return "UNKNOWN";
            break;
        }
    }
};

#endif // POKEMON_H
