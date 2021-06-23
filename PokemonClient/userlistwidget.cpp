#include "userlistwidget.h"
#include "ui_userlistwidget.h"
#include<QStandardItemModel>
#include<QDebug>
#include<QPainter>
UserListWidget::UserListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListWidget)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    connect(ui->pushButtonBack,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonBack.";
        emit back();
    });
    connect(ui->pushButtonCheck,&QPushButton::clicked,[=](){
        qDebug()<<"Press pushButtonCheck";
//        int idx=ui->listViewOnlineUser;
        QString userName="";
        if(idx>=0&&idx<onlineUser.count())
            userName=onlineUser.at(idx);
        else if(idx>=onlineUser.count()&&idx<onlineUser.count()+otherUser.count())
            userName=otherUser.at(idx-onlineUser.count());
        qDebug()<<"idx = " << idx<<"  ChcekName  = "<<userName;
        if(userName!="")
            emit checkPokemon(userName);
    });
    connect(ui->listViewOnlineUser,&QListView::clicked,[=](QModelIndex idxs){
        idx=idxs.row();
    });
}

UserListWidget::~UserListWidget()
{
    delete ui;
}
void UserListWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/preview.jpg");
    painter.drawPixmap(0,0,pix.width(),pix.height(),pix);
}
void UserListWidget::reflesh(){
    idx=-1;
    QStandardItemModel *model =new QStandardItemModel();
    int userCount =onlineUser.count();
    int otherCount=otherUser.count();
     for(int i = 0; i <userCount; i++)
     {
         QString string = static_cast<QString>(onlineUser.at(i));
         QStandardItem *item = new QStandardItem(string+"[在线]\n");
         model->appendRow(item);
     }
     for(int i=userCount;i<userCount+otherCount;i++)
     {
         QString string = static_cast<QString>(otherUser.at(i-userCount));
         QStandardItem *item = new QStandardItem(string+"[离线]\n");
         model->appendRow(item);
     }

    ui->listViewOnlineUser->setModel(model);
    //connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
}
