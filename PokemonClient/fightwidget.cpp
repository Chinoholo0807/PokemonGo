#include "fightwidget.h"
#include "ui_fightwidget.h"
#include<QDebug>
#include<QStandardItemModel>
#include"pokemon.h"
#include<QMessageBox>
#include<QPainter>
FightWidget::FightWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FightWidget)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    connect(ui->pushButtonBack,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonBack.";
        emit back();
    });
    //连接槽函数,点击己方小精灵后显示详细信息
    connect(ui->listViewYourPokemon,&QListView::clicked,[=](QModelIndex idx){
        Pokemon* p=userPokemonList->at(idx.row());
        if(p==nullptr)  return ;
        QString path=PokemonGenerate::picPath(p->getUid());
        if(path!="UNKNOWN"){//加载图片
            QPixmap pix;
            pix.load(path);
            pix.scaled(100,100);
            pix.scaled(ui->labelPic->size().width(),ui->labelPic->size().height());
            ui->labelPic->setScaledContents(true);//设置可变属性
            ui->labelPic->setPixmap(pix);

            ui->labelAttack->setText(QString("%1").arg(p->getAttack()));
            ui->labelDefinse->setText(QString("%1").arg(p->getDefinse()));
            ui->labelHP->setText(QString("%1").arg(p->getHP()));
            ui->labelSpeed->setText(QString("%1").arg(p->getSpeed()));
            pokemonA=p;

        }
    });
    //连接槽函数，点击敌方小精灵后显示详细信息
    connect(ui->listViewRivalPokemon,&QListView::clicked,[=](QModelIndex idx){
        Pokemon* p=rivalPokemonList.at(idx.row());
        if(p==nullptr)  return ;
        QString path=PokemonGenerate::picPath(p->getUid());
        if(path!="UNKNOWN"){//加载图片
            QPixmap pix;
            pix.load(path);
            pix.scaled(100,100);
            pix.scaled(ui->labelPic2->size().width(),ui->labelPic2->size().height());
            ui->labelPic2->setScaledContents(true);//设置可变属性
            ui->labelPic2->setPixmap(pix);
            ui->labelAttack2->setText(QString("%1").arg(p->getDAttack()));
            ui->labelDefinse2->setText(QString("%1").arg(p->getDDefinse()));
            ui->labelHP2->setText(QString("%1").arg(p->getDHP()));
            ui->labelSpeed2->setText(QString("%1").arg(p->getDSpeed()));
            pokemonB=p;

        }
    });
    //连接槽函数，点击升级赛后进入升级赛战斗
    connect(ui->pushButtonMode0,&QPushButton::clicked,[=](){
        //先检查是否选择了对战双方精灵
        //没有选择己方小精灵
        if(ui->listViewYourPokemon->currentIndex().row()<0){
            QMessageBox::information(this,"请求战斗无效","请选择你要出战的己方小精灵");
            return ;
        }
        //没有选择敌方小精灵
        if(ui->listViewRivalPokemon->currentIndex().row()<0){
            QMessageBox::information(this,"请求战斗无效","请选择你要对战的敌方小精灵");
            return ;
        }
        //进入对战，发送信号给stackWidget来切换界面
        int autoFlag=ui->radioButtonAuto->isChecked();
        emit readyFight(0,pokemonA,pokemonB,autoFlag);
    });
    //连接槽函数，点击决斗赛后进入决斗赛战斗
    connect(ui->pushButtonMode1,&QPushButton::clicked,[=](){
        //先检查是否选择了对战双方精灵
        //没有选择己方小精灵
        if(ui->listViewYourPokemon->currentIndex().row()<0){
            QMessageBox::information(this,"请求战斗无效","请选择你要出战的己方小精灵");
            return ;
        }
        //没有选择敌方小精灵
        if(ui->listViewRivalPokemon->currentIndex().row()<0){
            QMessageBox::information(this,"请求战斗无效","请选择你要对战的敌方小精灵");
            return ;
        }
        //进入对战，发送信号给stackWidget来切换界面
        int autoFlag=ui->radioButtonAuto->isChecked();
        emit readyFight(1,pokemonA,pokemonB,autoFlag);
    });

}
FightWidget::~FightWidget()
{
    delete ui;
}
void FightWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/preview.jpg");
    painter.drawPixmap(0,0,pix.width(),pix.height(),pix);
}
//随机生成敌方小精灵信息
void FightWidget::randGeneratePInfo(){
    //清空原有精灵
    int pCount =rivalPokemonList.count();
    for(int i=0;i<pCount;i++){
        if(rivalPokemonList.at(i)!=nullptr)
           delete rivalPokemonList.at(i);
    }
    rivalPokemonList.clear();

    srand(time(NULL));
    int PNum=rand()%5+4;//每次随机生成4-8只小精灵
    for(int i=0;i<PNum;i++){
        int uid=rand()%(PokemonGenerate::PNum);
        int level=rand()%MAX_LEVEL+1;
        Pokemon *p=PokemonGenerate::generatePokemon(uid,level,0);
        rivalPokemonList.append(p);
    }
}
//刷新显示
void FightWidget::reflesh(){
    //刷新界面显示
    ui->listViewYourPokemon->reset();
    QStringList list;
    QStandardItemModel *model =new QStandardItemModel();
    int pCount=userPokemonList->count();
    for(int i=0;i<pCount;i++){
        QString tmp=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
                arg(PokemonGenerate::name(userPokemonList->at(i)->getUid())).\
                arg(userPokemonList->at(i)->getLevel()).\
                arg(userPokemonList->at(i)->getExp());
       list.append(tmp);
    }
     for(int i = 0; i <pCount; i++)
     {
         QString string = static_cast<QString>(list.at(i));
         QStandardItem *item = new QStandardItem(string);

         model->appendRow(item);
     }
    ui->listViewYourPokemon->setModel(model);
    //将小精灵状态栏制空
    ui->labelPic->setScaledContents(true);//设置可变属性
    ui->labelPic->setPixmap(QPixmap(""));
    ui->labelAttack->setText("-");
    ui->labelDefinse->setText("-");
    ui->labelHP->setText("-");
    ui->labelSpeed->setText("-");
    ui->labelPic2->setScaledContents(true);//设置可变属性
    ui->labelPic2->setPixmap(QPixmap(""));
    ui->labelAttack2->setText("-");
    ui->labelDefinse2->setText("-");
    ui->labelHP2->setText("-");
    ui->labelSpeed2->setText("-");

    //系统随机生成小精灵
    randGeneratePInfo();
    //显示信息
    ui->listViewRivalPokemon->reset();
    QStringList list2;
    QStandardItemModel *model2 =new QStandardItemModel();
    pCount=rivalPokemonList.size();
    for(int i=0;i<pCount;i++){
        QString tmp=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
                arg(PokemonGenerate::name(rivalPokemonList.at(i)->getUid())).\
                arg(rivalPokemonList.at(i)->getLevel()).\
                arg(rivalPokemonList.at(i)->getExp());
       list2.append(tmp);
    }
     for(int i = 0; i < pCount; i++)
     {
         QString string = static_cast<QString>(list2.at(i));
         QStandardItem *item = new QStandardItem(string);
         model2->appendRow(item);
     }
    ui->listViewRivalPokemon->setModel(model2);
}
