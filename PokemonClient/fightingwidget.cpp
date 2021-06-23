#include "fightingwidget.h"
#include "ui_fightingwidget.h"
#include<QPushButton>
#include<QDebug>
#include<QMovie>
#include<QMessageBox>
#include<QPainter>
#include<QTimer>
FightingWidget::FightingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FightingWidget)
{
    ui->setupUi(this);
    //初始化界面信息，如技能名，图标和小精灵图片
    //初始化界面信息，如技能名，图标和小精灵图片

   // pokemonA=PokemonGenerate::generatePokemon(0,10,0);
   // pokemonB=PokemonGenerate::generatePokemon(1,10,0);
    opA=new QLabel(this);
    opB = new QLabel(this);
    action = new QSequentialAnimationGroup(this);
    control=new PokemonFightControl();
    //连接技能按钮和对应槽函数来释放技能

    //技能1按钮
    connect(ui->pushButtonSkill0,&QPushButton::clicked,[=](){
        pressSkillButton(0);
    });
    //技能2按钮
    connect(ui->pushButtonSkill1,&QPushButton::clicked,[=](){
        pressSkillButton(1);
    });
    //技能3按钮
    connect(ui->pushButtonSkill2,&QPushButton::clicked,[=](){
        pressSkillButton(2);
    });
    //技能4按钮
    connect(ui->pushButtonSkill3,&QPushButton::clicked,[=](){
        pressSkillButton(3);
    });
    //Discard按钮
    connect(ui->pushButtonDiscard,&QPushButton::clicked,this,&FightingWidget::pressDiscardButton);
    //战斗结束判定
    connect(action,&QSequentialAnimationGroup::finished,this,[=](){
        ui->pushButtonSkill0->setEnabled(true);
        ui->pushButtonSkill1->setEnabled(true);
        ui->pushButtonSkill2->setEnabled(true);
        ui->pushButtonSkill3->setEnabled(true);
        //更新血条
        ui->progressBarHPA->setValue(pokemonA->getHP());
        ui->labelHpAText->setText(QString("%1/%2")\
                                  .arg(pokemonA->getDHP()).\
                                  arg(pokemonA->getHP()));

        ui->progressBarHPB->setValue(pokemonB->getHP());
        ui->labelHpBText->setText(QString("%1/%2")\
                                  .arg(pokemonB->getDHP()).\
                                  arg(pokemonB->getHP()));
        qDebug()<<"Now can check button.";
        if(control->gameOver()){
            //隐藏小精灵gif
            opA->hide();
            opB->hide();
            stateText="战斗结束";
            ui->textBrowserDisplay->append(stateText);
            if(gameMode==1&&pokemonA->isDead())//决斗赛战败丢弃精灵
                dealDiscardPokemon();
            else
                emit gameIsOver(pokemonA,pokemonB,gameMode,-1);
        }else{
        if(autoFlag)
            pressSkillButton(rand()%4);
        }
    });
    //与动画相关的槽函数
    connect(control,&PokemonFightControl::skillDetermine,this,&FightingWidget::dealSkill);
    connect(control,&PokemonFightControl::hurtDetermine,this,&FightingWidget::dealHurt);
}

FightingWidget::~FightingWidget()
{
    delete ui;
}
void FightingWidget::paintEvent(QPaintEvent *event){
    QPixmap pix = QPixmap(":/res/fightingWidget.png").scaled(this->size());
    //设置背景
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
void FightingWidget::startBattle(Pokemon *A, Pokemon *B, int mode){
    gameMode=mode;
    pokemonA=A;
    A->readyFight();
    pokemonB=B;
    B->readyFight();
    control->startFight(pokemonA,pokemonB);
    ui->frameDiscard->hide();
    ui->pushButtonSkill0->setText(pokemonA->getSkillName(0)+"\n"+pokemonA->getSkillDesc(0));
    ui->pushButtonSkill1->setText(pokemonA->getSkillName(1)+"\n"+pokemonA->getSkillDesc(1));
    ui->pushButtonSkill2->setText(pokemonA->getSkillName(2)+"\n"+pokemonA->getSkillDesc(2));
    ui->pushButtonSkill3->setText(pokemonA->getSkillName(3)+"\n"+pokemonA->getSkillDesc(3));
    QString path=PokemonGenerate::picPath(pokemonA->getUid());
    if(path!="UNKNOWN"){//加载pokemonA图片
        QPixmap pix;
        pix.load(path);
        pix.scaled(100,100);
        pix.scaled(ui->labelPicA->size().width(),ui->labelPicA->size().height());
        ui->labelPicA->setScaledContents(true);//设置可变属性
        ui->labelPicA->setPixmap(pix);
    }
    path=PokemonGenerate::picPath(pokemonB->getUid());
    if(path!="UNKNOWN"){//加载pokemonB图片
        QPixmap pix;
        pix.load(path);
        pix.scaled(100,100);
        pix.scaled(ui->labelPicB->size().width(),ui->labelPicB->size().height());
        ui->labelPicB->setScaledContents(true);//设置可变属性
        ui->labelPicB->setPixmap(pix);
    }
    //加载双方精灵gif
    // 设置两个小精灵贴图
    path=PokemonGenerate::gifPath(pokemonA->getUid(),1);
    opA->show();
    opA->setMovie(new QMovie(path));
    opA->setGeometry(54, 204, 192, 192);
    opA->movie()->start();

    opB->show();
    path=PokemonGenerate::gifPath(pokemonB->getUid(),0);
    opB->setMovie(new QMovie(path));
    opB->setGeometry(554,204, 192, 192);
    opB->movie()->start();
    //设置HP
    int HPA=pokemonA->getDHP();
    ui->progressBarHPA->setRange(0,HPA);
    ui->progressBarHPA->setValue(HPA);
    ui->labelHpAText->setText(QString("%1/%2").arg(HPA).arg(HPA));
    int HPB=pokemonB->getDHP();
    ui->progressBarHPB->setRange(0,HPB);
    ui->progressBarHPB->setValue(HPB);
    ui->labelHpBText->setText(QString("%1/%2").arg(HPB).arg(HPB));
    ui->textBrowserDisplay->clear();
    stateText="战斗开始";
    ui->textBrowserDisplay->append(stateText);

    //若为auto模式，自动出招
    if(autoFlag)
        pressSkillButton(rand()%4);
    //若为手动模式，则接下来等待用户按下技能按钮选择对应按钮，触发相应槽函数
}
void FightingWidget::pressSkillButton(int idx){
    for(int i=0;i<action->animationCount();i++){
        delete action->takeAnimation(i);
    }
    action->clear();
    int idx2=rand()%4;
    stateText=QString("--------第[%1]回合--------").arg(control->getRound());
    ui->textBrowserDisplay->append(stateText);
    stateText=QString("PokemonA 使用技能%1[%2],PokemonB 使用技能%3[%4]").\
            arg(idx+1).arg(pokemonA->getSkillName(idx)).\
            arg(idx2+1).arg(pokemonB->getSkillName(idx2));
    ui->textBrowserDisplay->append(stateText);
    control->useSkill(idx,idx2);
    action->setLoopCount(1);
    ui->pushButtonSkill0->setEnabled(false);
    ui->pushButtonSkill1->setEnabled(false);
    ui->pushButtonSkill2->setEnabled(false);
    ui->pushButtonSkill3->setEnabled(false);
    qDebug()<<"Can not check button";
    action->start();

}
void FightingWidget::pressDiscardButton(){
    //检测用户是否选择了要丢弃的精灵
    int checkNum=-1;
    if(ui->radioButton0->isChecked())
        checkNum=idx[0];
    else if(ui->radioButton1->isChecked())
        checkNum=idx[1];
    else if(ui->radioButton2->isChecked())
        checkNum=idx[2];
    if(checkNum==-1)
        QMessageBox::information(this,"丢弃精灵失败","请选择要丢弃的精灵");
    else//获取要丢弃精灵在背包中的位置
        emit gameIsOver(pokemonA,pokemonB,gameMode,checkNum);
}
void FightingWidget::dealSkill(bool isLeft,bool critFlag,bool hitFlag){
    qDebug()<<"dealSkill isLeft:"<<isLeft<<" critFlag:"<<critFlag<<" hitFlag:"<<hitFlag;
    QPropertyAnimation * animation1;
    QPropertyAnimation * animation2;
    QPropertyAnimation * animation3;
    QPropertyAnimation * animation4;

    if(isLeft){
        if(hitFlag){
            stateText="PokemonA使用技能,技能[命中]";
            if(critFlag)
                stateText+="[暴击]";
        }
        else
            stateText="PokemonA使用技能,技能[未命中/对敌方无效]";
        ui->textBrowserDisplay->append(stateText);
        QRect rect1(opA->x(),opA->y(),opA->width(),opA->height());
        QRect rect2(opB->x()-opA->width()*0.5,opA->y(),opA->width(),opA->height());

        animation1=new QPropertyAnimation(opA,"geometry",this);
        animation1->setDuration(600);
        animation1->setStartValue(rect1);
        animation1->setEndValue(rect2);
        animation1->setEasingCurve(QEasingCurve::OutCubic);

        action->addAnimation(animation1);
        action->addPause(100);

        animation2=new QPropertyAnimation(opA,"geometry",this);
        animation2->setDuration(600);
        animation2->setStartValue(rect2);
        animation2->setEndValue(rect1);
        animation2->setEasingCurve(QEasingCurve::Linear);

        action->addAnimation(animation2);
        action->addPause(100);
//        con1=connect(animation2,&QPropertyAnimation::finished,[=](){
//            qDebug()<<"skill animation2 end";
//           ui->progressBarHPA->setValue(pokemonA->getHP());
//           ui->labelHpAText->setText(QString("%1/%2")\
//                                     .arg(pokemonA->getDHP()).\
//                                     arg(pokemonA->getHP()));
//        });
    }else{
        if(hitFlag){
            stateText="PokemonB使用技能,技能[命中]";
            if(critFlag)
                stateText+="[暴击]";
        }
        else
            stateText="PokemonB使用技能,技能[被闪避]";
        ui->textBrowserDisplay->append(stateText);
        QRect rect1(opB->x(),opB->y(),opB->width(),opB->height());
        QRect rect2(opA->x()+opB->width()*0.5,opB->y(),opB->width(),opB->height());
        animation3=new QPropertyAnimation(opB,"geometry",this);
        animation3->setDuration(600);
        animation3->setStartValue(rect1);
        animation3->setEndValue(rect2);
        animation3->setEasingCurve(QEasingCurve::OutCubic);

        action->addAnimation(animation3);
        action->addPause(100);
        animation4=new QPropertyAnimation(opB,"geometry",this);
        animation4->setDuration(600);
        animation4->setStartValue(rect2);
        animation4->setEndValue(rect1);
        animation4->setEasingCurve(QEasingCurve::Linear);
        action->addAnimation(animation4);
//        con2=connect(animation4,&QPropertyAnimation::finished,[=](){
//            qDebug()<<"skill animation4 end";
//           ui->progressBarHPB->setValue(pokemonB->getHP());
//           ui->labelHpBText->setText(QString("%1/%2")\
//                                     .arg(pokemonB->getDHP()).\
//                                     arg(pokemonB->getHP()));
//        });
        action->addPause(100);
    }
}
void FightingWidget::dealHurt(bool isLeft,int damage){
    qDebug()<<"dealHurt isLeft:"<<isLeft<<" damage:"<<damage;
    QPropertyAnimation * animation1;
    QPropertyAnimation * animation2;
    QPropertyAnimation * animation3;
    QPropertyAnimation * animation4;
    if(isLeft){      
        stateText=QString("PokemonA受到伤害[%1],剩余血量[%2]")\
                .arg(damage).arg(pokemonA->getHP());
        ui->textBrowserDisplay->append(stateText);
        QRect rect1(opA->x(),opA->y(),opA->width(),opA->height());
        QRect rect2(opA->x()-opA->width()*0.2,opA->y(),opA->width(),opA->height());

        animation1=new QPropertyAnimation(opA,"geometry",this);
        animation1->setDuration(200);
        animation1->setStartValue(rect1);
        animation1->setEndValue(rect2);
        animation1->setEasingCurve(QEasingCurve::Linear);

        action->addAnimation(animation1);
        action->addPause(50);

        animation2=new QPropertyAnimation(opA,"geometry",this);
        animation2->setDuration(200);
        animation2->setStartValue(rect2);
        animation2->setEndValue(rect1);
        animation2->setEasingCurve(QEasingCurve::Linear);
        action->addAnimation(animation2);
//        con3=connect(animation2,&QPropertyAnimation::finished,[=](){
//            qDebug()<<"hurt animation2 end";
//           ui->progressBarHPA->setValue(pokemonA->getHP());
//           ui->labelHpAText->setText(QString("%1/%2")\
//                                     .arg(pokemonA->getDHP()).\
//                                     arg(pokemonA->getHP()));
//        });
        action->addPause(100);
    }else{
        stateText=QString("PokemonB受到伤害[%1],剩余血量[%2]")\
                .arg(damage).arg(pokemonB->getHP());
        ui->textBrowserDisplay->append(stateText);
        QRect rect1(opB->x(),opB->y(),opB->width(),opB->height());
        QRect rect2(opB->x()+opB->width()*0.2,opB->y(),opB->width(),opB->height());

        animation3=new QPropertyAnimation(opB,"geometry",this);
        animation3->setDuration(200);
        animation3->setStartValue(rect1);
        animation3->setEndValue(rect2);
        animation3->setEasingCurve(QEasingCurve::Linear);

        action->addAnimation(animation3);
        action->addPause(50);

        animation4=new QPropertyAnimation(opB,"geometry",this);
        animation4->setDuration(200);
        animation4->setStartValue(rect2);
        animation4->setEndValue(rect1);
        animation4->setEasingCurve(QEasingCurve::Linear);
//        con4=connect(animation4,&QPropertyAnimation::finished,[=](){
//            qDebug()<<"hurt animation4 end";
//           ui->progressBarHPB->setValue(pokemonB->getHP());
//           ui->labelHpBText->setText(QString("%1/%2")\
//                                     .arg(pokemonB->getDHP()).\
//                                     arg(pokemonB->getHP()));
//        });
        action->addAnimation(animation4);
        action->addPause(100);
    }
}
void FightingWidget::dealDiscardPokemon(){
   int pCount=info->pCount;
   if(pCount ==1){//只有1只小精灵
       ui->radioButton1->hide();
       ui->radioButton2->hide();
       QString text=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
               arg(PokemonGenerate::name(userPokemonList->at(0)->getUid())).\
               arg(userPokemonList->at(0)->getLevel()).\
               arg(userPokemonList->at(0)->getExp());
       ui->radioButton0->setText(text);
       idx[0]=0;
   }else if(pCount ==2){//只有2只小精灵
       ui->radioButton2->hide();
       QString text=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
               arg(PokemonGenerate::name(userPokemonList->at(0)->getUid())).\
               arg(userPokemonList->at(0)->getLevel()).\
               arg(userPokemonList->at(0)->getExp());
       ui->radioButton0->setText(text);
       idx[0]=0;
       text=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
               arg(PokemonGenerate::name(userPokemonList->at(1)->getUid())).\
               arg(userPokemonList->at(1)->getLevel()).\
               arg(userPokemonList->at(1)->getExp());
       ui->radioButton1->setText(text);
       idx[1]=1;
   }else{//拥有小精灵数量>=3,随机选3只
      srand(time(NULL));
      idx[0]=rand()%pCount;
      idx[1]=rand()%pCount;
      while(idx[1]==idx[0])
          idx[1]=rand()%pCount;
      idx[2]=rand()%pCount;
      while(idx[2]==idx[1]||idx[2]==idx[0])
          idx[2]=rand()%pCount;
      QString text=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
              arg(PokemonGenerate::name(userPokemonList->at(idx[0])->getUid())).\
              arg(userPokemonList->at(idx[0])->getLevel()).\
              arg(userPokemonList->at(idx[0])->getExp());
      ui->radioButton0->setText(text);
      text=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
              arg(PokemonGenerate::name(userPokemonList->at(idx[1])->getUid())).\
              arg(userPokemonList->at(idx[1])->getLevel()).\
              arg(userPokemonList->at(idx[1])->getExp());
      ui->radioButton1->setText(text);
      text=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
              arg(PokemonGenerate::name(userPokemonList->at(idx[2])->getUid())).\
              arg(userPokemonList->at(idx[2])->getLevel()).\
              arg(userPokemonList->at(idx[2])->getExp());
      ui->radioButton2->setText(text);
   }
   ui->frameDiscard->show();
   ui->radioButton0->setChecked(true);
   ui->radioButton1->setChecked(true);
   ui->radioButton2->setChecked(true);
}


