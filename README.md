<!--
 * @Description: 
 * @Author: l
 * @Date: 2021-06-23 15:33:43
 * @LastEditors: l
 * @LastEditTime: 2021-06-23 15:41:34
 * @FilePath: \PokemonGo\README.md
-->
# PokemonGo
- 一个基于Qt的宠物小精灵对战系统

## 功能
1. 用户查询需求：
- 可以查看在线用户列表
- 可以查看某个用户的胜率
- 可以查看某个用户胜场次数和败场次数
- 可以查看用户拥有的宠物个数徽章（金银铜）和高级宠物徽章（金银铜）
- 可以查看所有成功注册用户拥有的精灵以及具体数据
- 可以查看离线用户

2. 游戏对战要求：
- 可以通过图形化界面进行战斗操作，选择小精灵的技能来达到操作战斗的目的
- 可以看到战斗过程中的各种数据
- 可以选择对战双方精灵和对战模式
- 若决斗赛战败，可以选择丢弃精灵，若失去最后一只精灵，服务器会随机再分配一只初始精灵
- 战斗具有随机性，拥有概率暴击和闪避等机制
- 战斗具有可操作性，可以选择自动战斗或者手动战斗
3. 用户注册与平台登录要求：
- 每个用户需要注册一个账号，用户名全局唯一，不能有任何两个用户名相同
- 注册时输入用户名和密码，点击注册进行注册，若注册成功服务器会随机分配3个等级为1的小精灵给新用户
- 要考虑注册失败的情况，比如注册用户名已存在
- 可以通过用户名与密码实现登录，登录后可以主动退出
- 若与服务器断开连接要让用户能察觉到，让用户重新连接
- 
## 用户使用指南
- 用户登录和注册

  ![image](https://user-images.githubusercontent.com/54203997/123056482-8ab0d080-d439-11eb-924a-ce0ddd3cb204.png)
  ![image](https://user-images.githubusercontent.com/54203997/123056550-98feec80-d439-11eb-8297-f6759a61503c.png)

- 查看用户背包

  ![image](https://user-images.githubusercontent.com/54203997/123056600-a4521800-d439-11eb-8e1e-6b033de37f62.png)
- 查看在线用户列表

  ![image](https://user-images.githubusercontent.com/54203997/123056646-b03dda00-d439-11eb-8adf-16eeb9da148b.png)
  ![image](https://user-images.githubusercontent.com/54203997/123056669-b6cc5180-d439-11eb-8ebe-67e2ff54cca3.png)
- 进入战斗大厅

  ![image](https://user-images.githubusercontent.com/54203997/123056756-cba8e500-d439-11eb-9f04-5d69598dd2ef.png)
- 战斗模拟

  ![image](https://user-images.githubusercontent.com/54203997/123056792-d6fc1080-d439-11eb-95d3-7341341f30fb.png)
  ![image](https://user-images.githubusercontent.com/54203997/123056802-d95e6a80-d439-11eb-8859-0769758bfc1c.png)
  ![image](https://user-images.githubusercontent.com/54203997/123056810-dbc0c480-d439-11eb-9bc7-9de0b8e523a4.png)

## 如何使用源代码生成可执行文件
1.	确保对应文件夹所在路径没有中文，将对应文件夹移动至纯英文目录下
2.	打开Qt Creator(编写时版本号为5.9.9，只要版本不是过低基本都能支持),选择打开文件，选择以 .pro 结尾的文件(PokemonClient.pro / PokemonServer.pro / PokemonTest. pro) ，
然后点击 Configure Project生成项目
 
3.	在debug模式下点击编译运行，便可得到可执行文件
 （注：若无法生成，则删除Debug文件夹和.pro.user结尾的文件，然后执行上述操作重新编译生成）
