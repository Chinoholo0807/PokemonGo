#include"dispatcher.h"
#include"define.h"
#include<QJsonDocument>
#include<QJsonObject>
Dispatcher::Dispatcher()
{

}

QJsonObject Dispatcher::dispatch(QJsonObject requestInfo){
    QJsonObject responseInfo;
    switch(requestInfo.take("type").toInt()){
    case LOGIN:
        responseInfo=loginHandle(requestInfo);
        break;
    case REGISTER:
        responseInfo=registerHandle(requestInfo);
        break;
    case USER_INFO:
        responseInfo=userInfoHandle(requestInfo);
        break;
    case ONLINE_USER:
        responseInfo=onlineUserHandle(requestInfo);
        break;
    case GAME_MODE0_RS:
        responseInfo=game0ResultHandle(requestInfo);
        break;
    case GAME_MODE1_RS:
        responseInfo=game1ResultHandle(requestInfo);
        break;
    default:
        responseInfo["type"]=SERVER_ERROR;
        break;
    }
    return responseInfo;
}
//登录处理逻辑
QJsonObject Dispatcher::loginHandle(QJsonObject &requestInfo){
    QJsonObject responseInfo;
    QString userName=requestInfo["user_name"].toString();
    QString password=requestInfo["password"].toString();
   // qDebug()<<"userName: "<<userName<<" password: "<<password;
    QSqlQuery query(parent->getDatabase());
    //在数据库中查找
    query.exec(QString("select * from user where user_name == '%1';").arg(userName));
    if(query.next()){//存在用户名，检测密码是否正确
   //     qDebug()<<"Find user name.";
        if(password == query.value(1).toString()){//密码正确
            if(!parent->isOnline(userName)){//该用户不在线，可以上线
                responseInfo.insert("type",LOGIN_SC);
                //将该用户标记为上线状态
                parent->addOnlineUser(userName,this->socket);
            }else{//该用户已经在线，无法再次登录
                responseInfo.insert("type",LOGIN_OL);
            }
        }else{//密码错误
           responseInfo.insert("type",LOGIN_WP);
        }
    }else{//不存在该用户名
        responseInfo.insert("type",LOGIN_WP);
    }
    return responseInfo;
}
//注册处理逻辑
QJsonObject Dispatcher::registerHandle(QJsonObject &requestInfo){
    QJsonObject responseInfo;
    QString userName=requestInfo["user_name"].toString();
    QString password=requestInfo["password"].toString();
   // qDebug()<<"userName: "<<userName<<" password: "<<password;
    QSqlQuery query(parent->getDatabase());
    //在数据库中查找
    query.exec(QString("select * from user where user_name == '%1';").arg(userName));
    if(!query.next()){//不存在用户名,允许注册
        query.exec(QString("insert into user values('%1','%2',0,0,3)").\
                   arg(userName).arg(password));
        responseInfo.insert("type",REGISTER_SC);
        //同时为新用户随机分配3只1级初始精灵
        srand(time(NULL));
        for(int i=0;i<3;i++){
            int id=rand()%10000;//分配id
            query.exec(QString("select * from pack where id ==%1;").arg(id));
            while(query.next()){//该id已经存在
                id=rand()%10000;//重新选值
                query.exec(QString("select * from pack where id ==%1;").arg(id));//再次查看
            }
            int uid=rand()%6;
            query.exec(QString("insert into pack values(%1,'%2',%3,1,0)").\
                       arg(id).arg(userName).arg(uid));
        }
    }else{//用户名已存在，不允许重复注册
        responseInfo.insert("type",REGISTER_WP);
    }
    return responseInfo;
}
//用户信息申请处理逻辑
QJsonObject Dispatcher::userInfoHandle(QJsonObject&requestInfo){
    QJsonObject responseInfo;
    QString userName=requestInfo["user_name"].toString();
    QSqlQuery query(parent->getDatabase());
    //在数据库中查找
    UserInfo userInfo;
    int uid,level,exp,id;
    query.exec(QString("select * from user where user_name == '%1';").arg(userName));
    if(query.next()){//存在用户名
        userInfo.userName=query.value(0).toString();
        userInfo.password=query.value(1).toString();
        userInfo.winCount=query.value(2).toInt();
        userInfo.lostCount=query.value(3).toInt();
        userInfo.pCount=0;
        query.exec(QString("select * from pack where user_name == '%1';").arg(userName));
        //查看精灵背包
        while(query.next()){
            uid=query.value(2).toInt();
            level=query.value(3).toInt();
            exp=query.value(4).toInt();
            id=query.value(0).toInt();
            userInfo.getPokemon(uid,level,exp,id);
        }
        responseInfo.insert("type",USER_INFO_SC);
        responseInfo.insert("user_info",UserInfo::UInfo2QJson(userInfo));
    }else//找不到用户名
        responseInfo.insert("type",SERVER_ERROR);

    return responseInfo;

}
//查看在线用户处理逻辑
QJsonObject Dispatcher::onlineUserHandle(QJsonObject & requestInfo){
    QJsonObject responseInfo;
    responseInfo.insert("type",ONLINE_USER_SC);
    QJsonArray userList;
    QJsonArray otherList;
    auto list =parent->getOnlineUser();
    auto list2 = parent->getOtherUser();
    for(auto &i:list){
        userList.append(i);
    }
    for(auto &i:list2){
        otherList.append(i);
    }
    responseInfo.insert("online_user",userList);
    responseInfo.insert("other_user",otherList);
    return responseInfo;
}
//同步用户升级赛战斗数据处理逻辑
QJsonObject Dispatcher::game0ResultHandle(QJsonObject&requestInfo){
    QJsonObject responseInfo;
    QString userName=requestInfo.value("user_name").toString();
    int id=requestInfo.value("id").toInt();
    int level=requestInfo.value("level").toInt();
    int exp=requestInfo.value("exp").toInt();
    int winCount=requestInfo.value("win_count").toInt();
    int lostCount=requestInfo.value("lost_count").toInt();
    QSqlQuery query(parent->getDatabase());
    query.exec(QString("select * from pack where user_name == '%1' and id == %2")\
               .arg(userName).arg(id));
    if(query.next()){
        query.exec(QString("update user set win_count = %1 where user_name == '%2'")\
                   .arg(winCount).arg(userName));
        query.exec(QString("update user set lost_count = %1 where user_name == '%2'")\
                   .arg(lostCount).arg(userName));
        query.exec(QString("update pack set level = %1 where id == %2")\
                   .arg(level).arg(id));
        query.exec(QString("update pack set exp = %1 where id == %2")\
                   .arg(exp).arg(id));
        responseInfo.insert("type",UPDATE_GAME0_SC);//同步成功
    }else{
        responseInfo.insert("type",SERVER_ERROR);//同步失败
    }
    return responseInfo;
}
//同步用户决斗赛战斗数据处理逻辑
QJsonObject Dispatcher::game1ResultHandle(QJsonObject&requestInfo){
    QJsonObject responseInfo;
    QString userName=requestInfo["user_name"].toString();
    int winFlag=requestInfo["win_flag"].toInt();
    int id=requestInfo.value("id").toInt();
    int level=requestInfo.value("level").toInt();
    int exp=requestInfo.value("exp").toInt();
    int winCount=requestInfo.value("win_count").toInt();
    int lostCount=requestInfo.value("lost_count").toInt();
    int pCount=requestInfo.value("pokemon_count").toInt();
    QSqlQuery query(parent->getDatabase());
    query.exec(QString("select * from pack where user_name == '%1' and id == %2")\
               .arg(userName).arg(id));
    if(query.next()){
        query.exec(QString("update user set win_count = %1 where user_name == '%2'")\
                   .arg(winCount).arg(userName));
        query.exec(QString("update user set lost_count = %1 where user_name == '%2'")\
                   .arg(lostCount).arg(userName));
        query.exec(QString("update user set pokemon_count = %1 where user_name == '%2'")\
                   .arg(pCount).arg(userName));
        query.exec(QString("update pack set level = %1 where id == %2")\
                   .arg(level).arg(id));
        query.exec(QString("update pack set exp = %1 where id == %2")\
                   .arg(exp).arg(id));
        if(winFlag){//胜利,获得精灵,分配id
            int newID=rand()%10000;//分配id
            query.exec(QString("select * from pack where id ==%1;").arg(newID));
            while(query.next()){//该id已经存在
                newID=rand()%10000;//重新选值
                query.exec(QString("select * from pack where id ==%1;").arg(newID));//再次查看
            }
            int newLevel=requestInfo.value("new_level").toInt();
            int newExp=requestInfo.value("new_exp").toInt();
            int newUid=requestInfo.value("new_uid").toInt();
            query.exec(QString("insert into pack values(%1,'%2',%3,%4,%5)").\
                       arg(newID).arg(userName).arg(newUid).arg(newLevel).arg(newExp));
            responseInfo.insert("new_id",newID);
        }else{//战败
            //从数据库中删除丢弃精灵信息
            int discardID=requestInfo.value("discard_id").toInt();
            query.exec(QString("delete from pack where id == %1 ").\
                       arg(discardID));
            if(pCount==0){//用户失去所有的精灵，重新分配一只精灵
                int pid=rand()%10000;//分配id
                query.exec(QString("select * from pack where id ==%1;").arg(pid));
                while(query.next()){//该id已经存在
                    pid=rand()%10000;//重新选值
                    query.exec(QString("select * from pack where id ==%1;").arg(pid));//再次查看
                }
                int puid=rand()%3;
                query.exec(QString("insert into pack values(%1,'%2',%3,1,0)").\
                           arg(pid).arg(userName).arg(puid));
                query.exec(QString("update user set pokemon_count =%1 where user_name =='%2'")\
                           .arg(1).arg(userName));
                responseInfo.insert("new_uid",puid);
                responseInfo.insert("new_id",pid);
                responseInfo.insert("new_level",1);
                responseInfo.insert("new_exp",0);
            }
        }
        responseInfo.insert("type",UPDATE_GAME1_SC);
    }else
        responseInfo.insert("type",SERVER_ERROR);//同步失败
    return responseInfo;
}
