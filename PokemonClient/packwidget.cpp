#include "packwidget.h"
#include "ui_packwidget.h"
#include<QDebug>
#include<QStandardItemModel>
#include<QStandardItem>
#include<QPainter>
PackWidget::PackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PackWidget)
{
    ui->setupUi(this);
    setFixedSize(800,600);
//    setAutoFillBackground(true);
//    QPalette palette;
//    QPixmap pixmap(":/res/1.jpg");
//    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(width(), height())));
//    setPalette(palette);

    connect(ui->pushButtonBack,&QPushButton::clicked,[=](){
        qDebug()<<"Enter pushButtonBack.";
        emit back();
    });
    connect(ui->listViewPokemon,&QListView::clicked,[=](QModelIndex idx){
        if(idx.row()<0||idx.row()>=userPokemonList.count())
            return;
        Pokemon *p=userPokemonList.at(idx.row());
        if(p==nullptr)
            return;
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
        }
    });
}

PackWidget::~PackWidget()
{
    delete ui;
}
void PackWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/preview.jpg");
    painter.drawPixmap(0,0,pix.width(),pix.height(),pix);
}
void PackWidget::reflesh(){
    //刷新界面显示
    qDebug()<<"Reflesh packWidget.";
    ui->listViewPokemon->reset();
    QStringList list;
    QStandardItemModel *model =new QStandardItemModel();
    for(int i=0;i<info.pCount;i++){
        QString tmp=QString(" 小精灵: %1\n 等级: %2\n 经验值: %3").\
                arg(PokemonGenerate::name(userPokemonList.at(i)->getUid())).\
                arg(userPokemonList.at(i)->getLevel()).\
                arg(userPokemonList.at(i)->getExp());
       list.append(tmp);
       userPokemonList.at(i)->printState();
    }
     for(int i = 0; i < info.pCount; i++)
     {
         QString string = static_cast<QString>(list.at(i));
         QStandardItem *item = new QStandardItem(string);

         model->appendRow(item);
     }
    ui->listViewPokemon->setModel(model);

    //将小精灵具体数据栏制空
    ui->labelPic->setScaledContents(true);//设置可变属性
    ui->labelPic->setPixmap(QPixmap(""));
    ui->labelAttack->setText("-");
    ui->labelDefinse->setText("-");
    ui->labelHP->setText("-");
    ui->labelSpeed->setText("-");

    //显示用户信息
    ui->labelUserName->setText(info.userName);
    ui->labelWinCount->setText(QString("%1").arg(info.winCount));
    ui->labelLostCount->setText(QString("%1").arg(info.lostCount));

    double winRate;
    if(info.winCount!=0||info.lostCount!=0)//防止除0
        winRate=(double)((double)info.winCount/(double)(info.winCount+info.lostCount));
    else
        winRate=0;
    ui->labelWinRate->setText(QString("%1\%").arg(winRate*100));
    ui->labelPCount->setText(QString("%1").arg(info.pCount));

    //显示奖牌

    //宠物个数勋章
    QPixmap pix;
    if(info.pCount>=15)
        pix=QPixmap(":/res/gold.png").scaled(ui->labelMedal0Pic->size());
    else if(info.pCount>=10)
        pix=QPixmap(":/res/silver.png").scaled(ui->labelMedal0Pic->size());
    else if(info.pCount>=5)
        pix=QPixmap(":/res/bronze.png").scaled(ui->labelMedal0Pic->size());
    else
        pix=QPixmap(":/res/none.png").scaled(ui->labelMedal0Pic->size());
    ui->labelMedal0Pic->setPixmap(pix);
    //高级宠物勋章
    int highLevelCount=0;
    for(int i=0;i<info.pCount;i++)
        if(info.pack.at(i).level==15)
            highLevelCount++;

    if(highLevelCount>=15)
        pix=QPixmap(":/res/gold.png").scaled(ui->labelMedal0Pic->size());
    else if(highLevelCount>=10)
        pix=QPixmap(":/res/silver.png").scaled(ui->labelMedal0Pic->size());
    else if(highLevelCount>=5)
        pix=QPixmap(":/res/bronze.png").scaled(ui->labelMedal0Pic->size());
    else
        pix=QPixmap(":/res/none.png").scaled(ui->labelMedal0Pic->size());
    ui->labelMedal1Pic->setPixmap(pix);
}
