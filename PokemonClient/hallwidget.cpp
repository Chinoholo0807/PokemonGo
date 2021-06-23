#include "hallwidget.h"
#include "ui_hallwidget.h"
#include<QPainter>
#include<QPixmap>
#include<QDebug>
HallWidget::HallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HallWidget)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    //连接槽函数
    connect(ui->pushButtonPack,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonPack";
        emit openPack();
    });
    connect(ui->pushButtonFight,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonFight";
        emit enterFight();
    });
    connect(ui->pushButtonUserList,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonUserList";
        emit checkUserList();
    });
    connect(ui->pushButtonQuit,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonQuit.";
        this->close();
        emit quit();
    });
}

HallWidget::~HallWidget()
{
    delete ui;
}
void HallWidget::paintEvent(QPaintEvent *event){
    //设置背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/hallWidget.jpg");
    painter.drawPixmap(-width()*0.2,-height()*0.5,pix.width(),pix.height(),pix);
}
