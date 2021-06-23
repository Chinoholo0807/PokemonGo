#ifndef DEFINE_H
#define DEFINE_H

/*---------设置客户端向服务器发送的请求类型--------*/
#define LOGIN 1 //用户登录请求
#define REGISTER 2 //用户注册请求
#define USER_INFO 3 //用户信息申请
#define ONLINE_USER 4 //查看在线用户申请

#define GAME_MODE0_RS 5//向服务器同步升级赛战斗结果
#define GAME_MODE1_RS 6//向服务器同步决斗赛战斗结果
/*--------------------------------------------*/


/*---------设置服务器向客户端返回的结果类型--------*/
#define LOGIN_SC 65 //用户登录申请成功，登录用户
#define LOGIN_WP 66 //用户登录申请失败，密码错误或者用户名错误
#define LOGIN_OL 67 //用户登录申请失败，用户已在线，不允许重复登录

#define REGISTER_SC 68 //用户注册申请成功，可以用注册账号登录用户
#define REGISTER_WP 69 //用户注册申请失败，用户名已存在

#define USER_INFO_SC 70 //用户信息申请成功，返回所需用户信息
#define ONLINE_USER_SC 71 //查看在线用户申请成功，返回在线用户

#define UPDATE_GAME0_SC 72//成功同步升级赛战斗结果
#define UPDATE_GAME1_SC 73//成功同步决斗赛战斗结果
#define SERVER_ERROR 80 //服务器返回未知错误
/*--------------------------------------------*/


#endif // DEFINE_H
