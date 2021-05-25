# ClusterChatSever


# 介绍



## 1 背景
 秋招在急，简历中没有一个合适的项目供面试官去展开提问和自己去练习。  而且在学习c++的过程中最大的问题就是没有合适的项目去融汇贯通， 不知道做什么才好，像java可以做很多有意思的后台项目， 但是c++貌似如果只是懂语法去做这些后台项目很难， 不知道从什么点出发去做。我想通过这个项目去掌握C++如何开发后台项目，不涉及分布式。  通过这个项目明白后台的开发流程， 让自己的c++能够做到实际开发的水平， 敢写c++。
 
 ## 2 任务
实现一个集群聊天服务器， 能够高并发的接收客服端的请求。 通过这个项目掌握
1. 掌握服务器的网络I/O模块，业务模块，数据模块分层的设计思想， ORM
2. 掌握C++ muduo网络库的编程以及实现原理
3. 掌握Json的编程应用
4. 掌握nginx配置部署tcp负载均衡器的应用以及原理
5. 掌握服务器中间件的应用场景和基于发布-订阅的redis编程实践以及应用原理
6. 掌握CMake构建自动化编译环境
7. 掌握Github管理项目
8. 熟悉现代c++， 尤其是绑定器和function等使用， 掌握面向对象设计的思想。 


 ## 3 行动

 ### 1. 环境配置
 1. vscode + 远程linux

 ### 2. 框架
  整体的框架如下， 多个服务器通过redis构建互通信息， 然后将多个服务器绑定到niginx上，与客户端交互。 服务器采用muduo库作为网络库底层， 以muduo库提供的回调作为业务层， 以自己封装的数据库类作为数据层的MVC结构进行开发。
最终实现了一个高并发的集群服务器。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522133518257.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdXBlbmcxOTk3MDExOQ==,size_16,color_FFFFFF,t_70)


 ### 3. 技术栈
 C++, linux , cmake , mysql , redis , nginx , muduo , gdb , git, json



 ### 4. 具体步骤

 1. 学习json和json开源库
 - +   数据交换语言是独立于编程语言的， 不同编程语言通信时通过json等交换语言进行。 json是一个比较简单的字节流交换语言。 通过key-value存储数据。 我们使用一个开源库 ， 他可以支持很多语言的json序列化和反序列化（也就是转换成数据类型或数据类型变成json）。用的时候也比较方便， 直接倒入一个头文件就行， 具体序列化实现都在这个文件文件中做了。 


2. muduo 网络库使用和学习
-  + 编译安装参照【1】， 这里面我已经装好了。 
-  +  muduo和众多网络库都是epoll+ 线程池的高性能服务器设计。 可以让我们直接关注到连接断开读写这几个事件的callback业务层， 请求来了底层自动调用callback。 
-  + 学会使用muduo开发一个基本的高并发服务器。 

3. cmake的使用与学习
- + 在vscode上配置cmake 辅助工具(界面操作+代码提示与检查)， 便于操作linux系统中的cmake. 
- + 基本的编译程序所做的命令例如：g++ -o server -g muduo_server.cpp xx.cpp -l/usr/include -L/usr/ib -Imuduo_net -lmuduo_base -lpthread  都可以在cmake中找到对应的配置选项， 去自动生成makefile。 
- +  cmakelists.txt 真的很强， 通过add_subdirectory(src) 去找子cmakelists.txt去编译。 


4. mysql 学习
- + mysql 安装与账号、权限配置
- + 创建业务需要用到的数据库和表。
```sql
CREATE TABLE IF NOT EXISTS user (id INT  AUTO_INCREMENT PRIMARY KEY  , name VARCHAR(50) NOT NULL UNIQUE ,  password VARCHAR(50) NOT NULL  , state   ENUM('online', 'offline')  DEFAULT 'offline'   )ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS friend (userid INT NOT NULL , friendid  INT NOT NULL  ,PRIMARY KEY (userid,friendid) )ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS allgroup (id INT  AUTO_INCREMENT PRIMARY KEY  , groupname VARCHAR(50) NOT NULL UNIQUE ,  groupdesc VARCHAR(200) DEFAULT ''  )ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS groupuser (groupid INT  NOT NULL , userid int  NOT NULL , grouprole   ENUM('creator', 'normal')  DEFAULT 'normal'  ,PRIMARY KEY (groupid,userid) )ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS offlinemessage (userid INT NOT NULL , message  VARCHAR(500) NOT NULL  )ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

4. 创建源代码目录
整个项目在src，include中分为后台和客户端。 各种目录的编译通过各级的cmakelist.txt去控制。 

5.  开发网络模块，搭建业务模块, 定义消息类型
- + 我们使用muduo库开发网络层，在网络层的回调中， 设置了单例的业务类， 根据自身消息的id在业务类的map回调函数表中找对应的处理函数， 至此网络模块不再变动添加，完全解耦。 
- + 消息类型如下： {"msgid" : 4 , "name" : "zhang san", "password" : "123456"}
- + 

6. 封装mysql数据库类
- +  像orm框架一样， 用类封装数据库操作， 这样当数据库的代码有所变动，例如表的变动， 不会变动业务代码。
- + 使用时候我们先将一个表定义成一个表类， 定义一个数据库连接类， 定义一个数据库封装类， 这个类包传入表类和包含数据库连接类， 在封装类中将表类中数据传递到数据库连接类（数据库连接类依赖mysql的客户端包）。
- + 简单的传入一两个数据的话，不用再添加一个数据类。 直接定义一个数据库操作类。

7. 编写业务模块类
- + 编写了数据库类之后，我们开始关注业务层。 注意数据库层一般是辅助业务层的， 所以不能在一开始就写完数据层，而是业务层缺什么补什么。 
- + 首先来看登录业务， 我们根据输入的信息 要去数据操作类中设置一个函数查找对应id的数据，判断是否账号密码匹配。 当匹配的话进入业务逻辑，例如是否已经登录等。 
- + 注册业务比较简单了， 将json数据放入user对象之后， 传入到数据库操作类定义的insert函数中对接数据库sql。
- +  添加客户端异常退出， 这个不是回调业务， 而是当服务器退出时候需要处理业务类的数据，清空在线状态。此外当我们调试阶段会主动关闭服务器， 导致里面的业务数据来不及更新。 因此添加ctrl +c 接收函数， 处理退出时候的业务重置。这个放到main函数里面。 但是好像不怎么解耦， 不过业务重置所在的类实单例业务类， 也没事。
- +  点对点聊天业务： 定义好发送信息的json格式之后，一个用户往另外一个用户发送的数据服务器收到之后会判断对方用户是否在线， 在线的话直接往对应的端口发送， 不在线的话就保存数据到离线数据库中。 
- + 离线业务比较简单，而且不是单独的业务，而是一个放在其他业务的子服务（例如当登录的时候先去调用这个数据库封装类看有无消息）。 而且其数据表比较简单， 所以我们不用设计数据对象， 直接封装数据库操作类， 传入的参数为整数和string即可。 
- + 加好友的业务 ： 当对应添加id的数据来的话， 对friend表进行insert， 此外还要将好友数据类的多表联合查询功能提供给登录业务， 一开始的时候就要显示当前登录账号的好友。 
- + 创建群的业务 ： group 是需要定义一个对象来存储数据的， group有两个表， 有一个数据库对象负责创建群的和管理这个群用户之间的关系。 
- +

 


## 4 结果


## 5 问题记录

### 编程问题

1. define 内容别的地方引用时候要在头文件里面define才能用到。 
2. g++  muduo_server.cpp  -o server -lmuduo_net  -lmuduo_base -lpthread  这个连接库是有依赖关系的， 最基础的在最前面。 
3. sudo  netstat  -tanp 查看端口对应的进程。 
4. 回调之前一直不理解， c++回调基本全部用bind+ functional 去代替了， 因为普通函数回调限制较多不能携带使用类变量。 回调贯穿了整个程序oop的解耦操作中， 当一个函数什么时候发生和发生时候怎么做不再一起，就要事先设置回调。 供其他程序调用发生时候启动。（其实可以写到其他程序那部分， 但是不能解耦。）这个项目中我们将业务封装到了业务类中， 当网络这块回调被epoller-wait启动后根据数据的id拿到业务类的对应方法进行调用(通过bind+ fc+ map)。 业务再怎么改， 这边代码都不动了。 真正解耦。 
5. 头文件负责定义类还有添加头文件（定义时候需要用到的） ， 注意只需要编译一次。 对应的.cpp 负责具体实现， 当我们将其编译成库的时候， 就只需要头文件了， 具体源码实现都在.so中， 不用库的话， 也会自动去找.cpp的实现。当然我们有时候不需要这个cpp文件，类直接在hpp里面定义并声明完成。  
6. 当形参变量和类成员变量的名字一样时候，一定要加this区分。 
7. 如何设计开源文件的目录， bin放可执行文件  ， lib是生成的库文件 ， include是头文件， src 是源码， build 项目编译时候产生的临时文件， test放的代码， cmakelist.txt 设计编译文件的规则,  autobuild.sh 自动编译， readme.md 。 
8. 公网链接的时候， 服务器绑定自己内网网卡的地址， 客户端访问服务器所连接的公网地址， 请求过来之后自然会通过路由找到内网所在的网卡。建立链接。 
9. 当程序从看的角度找不到问题， 就只能调试了， gdb打断点break到出问题的之前点， 然后run， next，排除问题。 而且很多知名的开源代码会经常使用基类指针的运行时多态， 如果不走运行调试分析， 很难理清整体的框架。 
10. 


### 设计问题
1. 如果我们还需要扩展的话， 需要将这些服务拆分，通过RPC框架注册成RPC服务， 然后供客户端请求， 有时候一个请求需要多个服务交互完成，这时候就需要zookeeper 注册中心，做服务的管理和统计， 让各个服务能够及时的响应和并保持一致性。但是很简单的一个问题， 贪多就不能精。 在这个找实习的节骨眼上， 做分布式明显不是自己现阶段的主要矛盾。
2.  线程数量和cpu核数量请保持一致， 这样避免不同核时候， 线程调用上下文调用过慢。 
3. 这个项目并没有使用数据库连接池， 因此每次查询都要创建数据库连接类效率比较低效。 
4.  用户和群的关系是多对多， 因此必须有一个中间表反应多对多之间的关系， 这里我们使用了一个id和组id联合主键的表作为中间表， 反应了用户和表的关系。 这是表的设计问题， 属于数据库开发工程师去做的， 你不用去设计出来这个表， 数据库开发工程师会给你理清里面的关系，给出表的定义。 
5.  创建群等操作没有ack， 我们可以自己添加。 
6. 不用担心json输入错误，我们调试时候是自己手动输入json， 而实际发送信息的是客户端， 到时候发送的格式是固定的。 


# How use it ?
1.  

#   参考文献
1. https://blog.csdn.net/QIANGWEIYUAN/article/details/89023980
2. 服务器编程教程

#   版本记录