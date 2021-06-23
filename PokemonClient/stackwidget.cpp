#include "stackwidget.h"
#include "ui_stackwidget.h"

StackWidget::StackWidget(Client *userClient,QWidget *parent) :
    QWidget(parent),
    client(userClient),
    ui(new Ui::StackWidget),
    hallWidget(new HallWidget(this)),
    fightWidget(new FightWidget(this)),
    fightingWidget(new FightingWidget(this)),
    packWidget(new PackWidget(this)),
    userListWidget(new UserListWidget(this))
{
    ui->setupUi(this);
    initWidget();
}

StackWidget::~StackWidget()
{
    //子窗口在对象树上，无需手动delete
//    delete ui;
//    delete stackedWidget;
//    delete hallWidget;
//    delete fightWidget;
//    delete fightingWidget;
//    delete packWidget;
//    delete userListWidget;
}
//设置当前界面
void StackWidget::setCurrentIndex(int idx){
    if(idx<stackedWidget->count()&&idx>=0)
        stackedWidget->setCurrentIndex(idx);
}
//返回大厅
void StackWidget::BackToHall(){
    setCurrentIndex(0);
}
//尝试服务器同步信息
bool StackWidget::trySynchronous(){
    QJsonObject requestInfo;
    requestInfo.insert("type",USER_INFO);
    requestInfo.insert("user_name",client->getUserName());
    qDebug()<<"Try to get user["+client->getUserName()+"] info....";
    QJsonObject responseInfo=client->sendMsg(requestInfo);
    if(responseInfo.value("type").toInt() == USER_INFO_SC){
        qDebug()<<"Get user info succeeded.";
        //将得到的responseInfo从QJsonObject转换为UserInfo
        userInfo=UserInfo::QJson2UInfo(responseInfo.value("user_info").toObject());
        userInfo.pCount=userInfo.pack.count();
        pokemonPack.clear();//实例化小精灵
        for(int i=0;i<userInfo.pCount;i++){
            int uid=userInfo.pack.at(i).uid;
            int level=userInfo.pack.at(i).level;
            int exp=userInfo.pack.at(i).exp;
            Pokemon* realPokemon=PokemonGenerate::generatePokemon(uid,level,exp);
            pokemonPack.append(realPokemon);
        }
        return true;
    }else{
        qDebug()<<"Get user info failed.";
        QMessageBox::information(this,"获取用户信息失败","请重新登录");
        return false;
    }
}
//申请进入用户背包界面
void StackWidget::tryEnterPackWidget(){
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    qDebug()<<"Try to enter packWidget...";
    //向服务器发起用户信息申请，来同步用户信息
    if(!trySynchronous()){
        QMessageBox::information(this,"同步用户信息失败","请重新登录");
        return ;//不连接对应槽函数
    }
    packWidget->setUserInfo(userInfo,pokemonPack);
    //刷新页面
    packWidget->reflesh();
    //跳转页面
    setWindowTitle(title+"用户背包");
    qDebug()<<"Enter packWidget.";
    setCurrentIndex(3);
}
//检查其他用户的精灵
void StackWidget::tryCheckOtherUser(QString userName){
    QString title=QString("Pokemon 当前用户:[%1] 用户背包所有者:[%2]").arg(client->getUserName()).arg(userName);
    qDebug()<<"Try to enter packWidget...";
    QJsonObject requestInfo;
    requestInfo.insert("type",USER_INFO);
    requestInfo.insert("user_name",userName);
    qDebug()<<"Try to get user["+userName+"] info....";
    QJsonObject responseInfo=client->sendMsg(requestInfo);
    UserInfo userInfo1;//用户信息
    QList<Pokemon*>pokemonPack1;//利用用户信息中的pack实例化的小精灵列表
    if(responseInfo.value("type").toInt() == USER_INFO_SC){
        qDebug()<<"Get user info succeeded.";
        //将得到的responseInfo从QJsonObject转换为UserInfo
        userInfo1=UserInfo::QJson2UInfo(responseInfo.value("user_info").toObject());
        userInfo1.pCount=userInfo1.pack.count();
        pokemonPack1.clear();//实例化小精灵
        for(int i=0;i<userInfo1.pCount;i++){
            int uid=userInfo1.pack.at(i).uid;
            int level=userInfo1.pack.at(i).level;
            int exp=userInfo1.pack.at(i).exp;
            Pokemon* realPokemon=PokemonGenerate::generatePokemon(uid,level,exp);
            pokemonPack1.append(realPokemon);
        }
       packWidget->setUserInfo(userInfo1,pokemonPack1);
       //刷新页面
       packWidget->reflesh();
       //跳转页面
       setWindowTitle(title+"用户背包");
       qDebug()<<"Enter packWidget.";
       setCurrentIndex(3);
    }else{
        qDebug()<<"Get user info failed.";
        QMessageBox::information(this,"获取用户信息失败","请重新登录");

    }
}
//申请进入战斗大厅
void StackWidget::tryEnterFightWidget(){
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    qDebug()<<"Try to enter fightWidget...";
    fightWidget->setUserInfo(&userInfo,&pokemonPack);
    fightWidget->randGeneratePInfo();//刷新敌方小精灵列表
    fightWidget->reflesh();//刷新页面显示
    setWindowTitle(title+"战斗大厅");
    qDebug()<<"Enter fightWidget.";
    setCurrentIndex(1);
}
//申请进入战斗场景
void StackWidget::tryEnterFightingWidget(int mode,Pokemon*A,Pokemon*B,int autoFlag){
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    qDebug()<<"Try to enter fightingWidget...";
    fightingWidget->setUserInfo(&userInfo,&pokemonPack);
    fightingWidget->setFlag(autoFlag);
    fightingWidget->startBattle(A,B,mode);//准备战斗
    if(mode==0)
        setWindowTitle(title+"升级赛");
    else if(mode ==1)
        setWindowTitle(title+"决斗赛");

    setCurrentIndex(2);
}
//申请进入在线用户场所
void StackWidget::tryEnterUserListWidget(){
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    qDebug()<<"Try to enter userListWidget...";
    //向服务器发起在线用户申请，查看在线用户
    QJsonObject requestInfo;
    requestInfo.insert("type",ONLINE_USER);
    QJsonObject responseInfo=client->sendMsg(requestInfo);
    if(responseInfo.value("type")==ONLINE_USER_SC){//获取在线用户成功
        QJsonArray array=responseInfo.value("online_user").toArray();
        int userCount=array.count();
        QStringList userList;
        for(int i=0;i<userCount;i++){
            userList.append(array.at(i).toString());
        }
        userListWidget->setOnlineUser(userList);
        array=responseInfo.value("other_user").toArray();
        userCount=array.count();
        QStringList otherList;
        for(int i=0;i<userCount;i++){
            otherList.append(array.at(i).toString());
        }
        userListWidget->setOtherUser(otherList);
        userListWidget->reflesh();
        setWindowTitle(title+"在线用户");
        qDebug()<<"Enter userListWidget.";
        setCurrentIndex(4);
    }else {
        qDebug()<<"Enter userListWidget failed.";
        QMessageBox::information(this,"获取在线用户失败",\
                                 QString("错误代码#%1").arg(responseInfo.value("type").toInt()));
    }
}
//申请从战斗场所返回战斗大厅,处理战斗结果
void StackWidget::tryBackToHallFromFightingWidget(Pokemon*pokemonA,Pokemon*pokemonB,int gameMode,int packIdx){
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    qDebug()<<"Deal game over.";
    qDebug()<<"Game mode :"<<gameMode;
    qDebug()<<"pCount"<<userInfo.pCount;
    qDebug()<<"userInfo-----------";
    for(int i=0;i<userInfo.pack.size();i++){
        qDebug()<<"id "<<userInfo.pack.at(i).id<<" uid "<<userInfo.pack.at(i).uid\
               <<" level "<<userInfo.pack.at(i).level;
    }
    qDebug()<<"PokemonList--------";
    for(int i=0;i<pokemonPack.size();i++){
        qDebug()<<"uid "<<pokemonPack.at(i)->getUid()<<" level "<<pokemonPack.at(i)->getLevel();
    }
    int gainExp=100+(pokemonB->getLevel()*300/pokemonA->getLevel());
    int idx,discardID;
    if(gameMode==0){//升级赛
        qDebug()<<"Deal game mode0.";
        if(pokemonB->isDead()){//用户胜利
            pokemonA->tryLevelUp(gainExp);//小精灵获得经验
            userInfo.winCount++;//胜场+1
        }else{//战败
            pokemonA->tryLevelUp(gainExp*0.5);//小精灵获得经验减半
            userInfo.lostCount++;//败场+1
        }
        //更新用户信息
        for(idx=0;idx<pokemonPack.count();idx++){
            if(pokemonPack.at(idx)==pokemonA)
                break;
        }
        userInfo.pack[idx].exp=pokemonA->getExp();
        userInfo.pack[idx].level=pokemonA->getLevel();
    }else if(gameMode==1){//决斗赛
        if(pokemonB->isDead()){//用户胜利，获得对战精灵
            pokemonA->tryLevelUp(gainExp);//小精灵获得经验
            userInfo.winCount++;//胜场+1
            //获得小精灵,向服务器同步时会得到新的id
            userInfo.getPokemon(pokemonB->getUid(),pokemonB->getLevel(),0,0);
            pokemonPack.append(PokemonGenerate::generatePokemon(pokemonB->getUid(),pokemonB->getLevel(),0));
            //更新userInfo中信息
            for(idx=0;idx<pokemonPack.count();idx++){
                if(pokemonPack.at(idx)==pokemonA)
                    break;
            }
            userInfo.pack[idx].exp=pokemonA->getExp();
            userInfo.pack[idx].level=pokemonA->getLevel();
        }else{//用户战败，失去精灵
            pokemonA->tryLevelUp(gainExp*0.1);//小精灵获得经验
            userInfo.lostCount++;//胜场+1
            qDebug()<<"Lost pokemon which packIdx= "<<packIdx;
            //更新userInfo中信息
            for(idx=0;idx<pokemonPack.count();idx++){
                if(pokemonPack.at(idx)==pokemonA)
                    break;
            }
            userInfo.pack[idx].exp=pokemonA->getExp();
            userInfo.pack[idx].level=pokemonA->getLevel();
            //保存discardID,由于信息还有用，在接受到responseInfo后再析构对象
            userInfo.pCount--;
            discardID=userInfo.pack.at(packIdx).id;
            qDebug()<<"discardID: "<<discardID;

        }
    }

    //向服务器发起同步信息
    QJsonObject requestInfo;
    if(gameMode==0){//升级赛
        requestInfo.insert("type",GAME_MODE0_RS);
        requestInfo.insert("user_name",client->getUserName());
        requestInfo.insert("id",userInfo.pack.at(idx).id);
        requestInfo.insert("level",userInfo.pack.at(idx).level);
        requestInfo.insert("exp",userInfo.pack.at(idx).exp);
        requestInfo.insert("win_count",userInfo.winCount);
        requestInfo.insert("lost_count",userInfo.lostCount);

        qDebug()<<"Try to get user["+client->getUserName()+"] info....";
        QJsonObject responseInfo=client->sendMsg(requestInfo);

        if(responseInfo.value("type").toInt()==UPDATE_GAME0_SC){//同步成功
            qDebug()<<"update game result succeeded.";
            if(pokemonB->isDead()){
                QString text=QString("获得经验: %1\n小精灵: %2\n等级: %3\n经验: %4\n").\
                        arg(gainExp).\
                        arg(PokemonGenerate::name(pokemonA->getUid())).\
                        arg(pokemonA->getLevel()).\
                        arg(pokemonA->getExp());
                QMessageBox::information(this,"升级赛胜利",text,QMessageBox::Ok);
            }else{
                QString text=QString("获得经验: %1\n小精灵: %2\n等级: %3\n经验: %4\n").\
                        arg(gainExp*0.5).\
                        arg(PokemonGenerate::name(pokemonA->getUid())).\
                        arg(pokemonA->getLevel()).\
                        arg(pokemonA->getExp());
                QMessageBox::information(this,"升级赛战败",text,QMessageBox::Ok);
            }
        }else{//同步失败
            qDebug()<<"update game result failed.";
            QMessageBox::information(this,"服务器同步失败","---请重新登录---");
            exit(1);
        }
    }else if(gameMode==1){//决斗赛
        requestInfo.insert("type",GAME_MODE1_RS);
        requestInfo.insert("user_name",client->getUserName());
        requestInfo.insert("id",userInfo.pack.at(idx).id);
        requestInfo.insert("level",userInfo.pack.at(idx).level);
        requestInfo.insert("exp",userInfo.pack.at(idx).exp);
        requestInfo.insert("win_count",userInfo.winCount);
        requestInfo.insert("lost_count",userInfo.lostCount);
        requestInfo.insert("pokemon_count",userInfo.pCount);
        if(pokemonB->isDead()){//胜利,获取的新的小精灵的信息
            requestInfo.insert("new_uid",userInfo.pack.back().uid);
            requestInfo.insert("new_level",userInfo.pack.back().level);
            requestInfo.insert("new_exp",userInfo.pack.back().exp);
            requestInfo.insert("win_flag",1);
        }else{//战败,丢弃的小精灵的id
            requestInfo.insert("win_flag",0);
            requestInfo.insert("discard_id",discardID);
        }
        qDebug()<<"Try to get user["+client->getUserName()+"] info....";
        QJsonObject responseInfo=client->sendMsg(requestInfo);
        //处理服务器回复信息
        qDebug()<<requestInfo;
        qDebug()<<responseInfo;
        if(responseInfo.value("type").toInt()==UPDATE_GAME1_SC){//同步成功
            if(pokemonB->isDead()){//胜利处理
                int newID=responseInfo.value("new_id").toInt();
                userInfo.pack.back().id=newID;//获取服务器分配新的uid
                QString text=QString("获得经验: %1\n小精灵: %2\n等级: %3\n经验: %4\n获得对战精灵[%5]").\
                        arg(gainExp).\
                        arg(PokemonGenerate::name(pokemonA->getUid())).\
                        arg(pokemonA->getLevel()).\
                        arg(pokemonA->getExp()).\
                        arg(PokemonGenerate::name(pokemonB->getUid()));
                QMessageBox::information(this,"决斗赛胜利",text,QMessageBox::Ok);
            }else{//失败处理
                //析构精灵对象，丢弃小精灵
                delete pokemonPack[packIdx];
                pokemonPack.removeAt(packIdx);
                userInfo.pack.removeAt(packIdx);
                if(userInfo.pCount==0){//失去所有精灵，服务器重新分配一只精灵
                    int pid=responseInfo.value("new_id").toInt();
                    int puid=responseInfo.value("new_uid").toInt();
                    int plevel=responseInfo.value("new_level").toInt();
                    int pexp=responseInfo.value("new_exp").toInt();
                    userInfo.getPokemon(puid,plevel,pexp,pid);
                    pokemonPack.append(PokemonGenerate::generatePokemon(puid,plevel,pexp));
                    //提示一下用户新分配了一只精灵
                    QString text=QString("由于您失去了最后一只小精灵，系统为您重新分配了一只小精灵\n小精灵: %2\n等级: %3\n经验: %4\n").\
                            arg(PokemonGenerate::name(puid)).\
                            arg(plevel).\
                            arg(pexp);
                    QMessageBox::information(this,"决斗赛战败",text,QMessageBox::Ok);
                }
            }
            qDebug()<<"update game result succeeded.";
        }else{
            qDebug()<<"update game result failed.";
            QMessageBox::information(this,"服务器同步失败","---请重新登录---");
            exit(1);
        }
    }
    qDebug()<<"pCount"<<userInfo.pCount;
    qDebug()<<"userInfo-----------";
    for(int i=0;i<userInfo.pack.size();i++){
        qDebug()<<"id "<<userInfo.pack.at(i).id<<" uid "<<userInfo.pack.at(i).uid\
               <<" level "<<userInfo.pack.at(i).level;
    }
    qDebug()<<"PokemonList--------";
    for(int i=0;i<pokemonPack.size();i++){
        qDebug()<<"uid "<<pokemonPack.at(i)->getUid()<<" level "<<pokemonPack.at(i)->getLevel();
    }
    fightWidget->reflesh();
    setWindowTitle(title+"战斗大厅");
    setCurrentIndex(1);
}
//申请返回大厅
void StackWidget::tryBackToHall(){
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    qDebug()<<"Back to hall.";
    setWindowTitle(title+"游戏大厅");
    BackToHall();
}
//初始化窗口，包括UI和槽函数
void StackWidget::initWidget(){
    //初始化stackedWidget
    setFixedSize(800,600);
    stackedWidget= new QStackedWidget(this);
    stackedWidget->setFixedSize(800,600);
    stackedWidget->addWidget(hallWidget);//游戏大厅界面 0
    stackedWidget->addWidget(fightWidget);//战斗大厅界面 1
    stackedWidget->addWidget(fightingWidget);//战斗场景界面 2
    stackedWidget->addWidget(packWidget);//用户背包界面 3
    stackedWidget->addWidget(userListWidget);//用户列表界面 4
    stackedWidget->setCurrentIndex(0);//默认为游戏大厅界面
    QString title=QString("Pokemon 当前用户:[%1] ").arg(client->getUserName());
    setWindowTitle(title+"游戏大厅");
    setWindowIcon(QIcon(":/res/icon.png"));

    //向服务器发起用户信息申请，来同步用户信息
    if(!trySynchronous()){
        QMessageBox::information(this,"同步用户信息失败","请重新登录");
        return ;//不连接对应槽函数
    }

    //连接对应槽函数

    //申请进入用户背包界面
    connect(hallWidget,&HallWidget::openPack,this,&StackWidget::tryEnterPackWidget);

    //申请进入战斗大厅
    connect(hallWidget,&HallWidget::enterFight,this,&StackWidget::tryEnterFightWidget);

    //接受到来自战斗大厅的战斗请求，申请进入战斗场景
    connect(fightWidget,&FightWidget::readyFight,this,&StackWidget::tryEnterFightingWidget);

    //申请查看在线用户列表
    connect(hallWidget,&HallWidget::checkUserList,this,&StackWidget::tryEnterUserListWidget);

    //从战斗场所返回战斗大厅,处理战斗结果
    connect(fightingWidget,&FightingWidget::gameIsOver,this,&StackWidget::tryBackToHallFromFightingWidget);

    //申请查看其他用户精灵
    connect(userListWidget,&UserListWidget::checkPokemon,this,&StackWidget::tryCheckOtherUser);
    //退出游戏
    connect(hallWidget,&HallWidget::quit,[=](){
        qDebug()<<"Quit the game.";
        this->close();
    });

    //返回大厅
    connect(fightWidget,&FightWidget::back,this,&StackWidget::tryBackToHall);
    connect(packWidget,&PackWidget::back,this,&StackWidget::tryBackToHall);
    connect(userListWidget,&UserListWidget::back,this,&StackWidget::tryBackToHall);

}
//发送请求给客户端，返回回复信息
QJsonObject StackWidget::sendRequestToServer(QJsonObject &requestInfo){
    return client->sendMsg(requestInfo);//返回回复信息
}
