#ifndef USERINFO_H
#define USERINFO_H
#include<QString>
#include<QList>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
//存储单个小精灵信息
typedef struct PokemonInfo{
    int uid;//小精灵uid
    int level;//等级
    int exp;//当前经验
    int id;//小精灵在数据库中的特有id,一个id对应一个用户的某一个小精灵
}PInfo;
//存储用户信息

struct UserInfo{
    QString userName;//用户名
    QString password;//密码
    int winCount;//战斗胜利次数
    int lostCount;//战斗失败次数
    int pCount;//拥有小精灵个数
    QList<PInfo> pack;//用户背包，利用小精灵uid+小精灵等级+当前经验就能实例化一只小精灵实体
    UserInfo():userName("UNKNOWN"),password(""),winCount(0),lostCount(0),pCount(0){}
    void getPokemon(int uid,int level,int exp,int id){
        pack.append({uid,level,exp,id});
        pCount++;
    }
    void updatePokemon(int idx,int level,int exp){
        if(idx<0||idx>=pack.count())    return ;
        pack[idx].level=level;
        pack[idx].exp=exp;
    }
    UserInfo & operator =(const UserInfo & that){//重载运算符=
        if(this!=&that){
        userName=that.userName;
        password=that.password;
        winCount=that.winCount;
        lostCount=that.lostCount;
        pCount=that.pCount;
        pack=that.pack;
        }
        return *this;
    }
    //将userInfo转化为QJsonObject
    static QJsonObject UInfo2QJson(UserInfo info){
        QJsonObject ret;
        QJsonArray packList;
        for(auto &i:info.pack){
            QJsonObject tmp;
            tmp.insert("level",i.level);
            tmp.insert("uid",i.uid);
            tmp.insert("exp",i.exp);
            tmp.insert("id",i.id);
            packList.append(tmp);
        }
        ret.insert("user_name",info.userName);
        ret.insert("password",info.password);
        ret.insert("win_count",info.winCount);
        ret.insert("lost_count",info.lostCount);
        ret.insert("pack_list",packList);
        ret.insert("pokemon_count",info.pCount);
        return ret;
    }
    //将QJsonObject转化为userInfo
    static UserInfo QJson2UInfo(QJsonObject json){
        UserInfo ret;
        ret.userName=json["user_name"].toString();
        ret.password=json["password"].toString();
        ret.winCount=json["win_count"].toInt();
        ret.lostCount=json["lost_count"].toInt();
        ret.pCount=json["pokemon_count"].toInt();
        ret.pack.clear();
        QJsonArray array=json["pack_list"].toArray();
        for(int i=0;i<array.count();i++){
            PInfo tmp;
            tmp.exp =array.at(i).toObject()["exp"].toInt();
            tmp.uid=array.at(i).toObject()["uid"].toInt();
            tmp.level=array.at(i).toObject()["level"].toInt();
            tmp.id=array.at(i).toObject()["id"].toInt();
            ret.pack.append(tmp);
        }
        return ret;
    }
};



#endif // USERINFO_H
