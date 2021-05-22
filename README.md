# ClusterChatSever


# 介绍

## 背景
 秋招在急，简历中没有一个合适的项目供面试官去展开提问和自己去练习。  而且在学习c++的过程中最大的问题就是没有合适的项目去融汇贯通， 不知道做什么才好，像java可以做很多有意思的后台项目， 但是c++貌似如果只是懂语法去做这些后台项目很难， 不知道从什么点出发去做。我想通过这个项目去掌握C++如何开发后台项目，不涉及分布式。  通过这个项目明白后台的开发流程， 让自己的c++能够做到实际开发的水平， 敢写c++。
 
 ## 任务
实现一个集群聊天服务器， 能够高并发的接收客服端的请求。 通过这个项目掌握
1. 掌握服务器的网络I/O模块，业务模块，数据模块分层的设计思想
2. 掌握C++ muduo网络库的编程以及实现原理
3. 掌握Json的编程应用
4. 掌握nginx配置部署tcp负载均衡器的应用以及原理
5. 掌握服务器中间件的应用场景和基于发布-订阅的redis编程实践以及应用原理
6. 掌握CMake构建自动化编译环境
7. 掌握Github管理项目


 ## 行动

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
 




# 问题记录

## 编程问题

1. define 内容别的地方引用时候要在头文件里面define才能用到。 
2. g++  muduo_server.cpp  -o server -lmuduo_net  -lmuduo_base -lpthread  这个连接库是有依赖关系的， 最基础的在最前面。 
3. sudo  netstat  -tanp 查看端口对应的进程。 


## 设计问题
1. 如果我们还需要扩展的话， 需要将这些服务拆分，通过RPC框架注册成RPC服务， 然后供客户端请求， 有时候一个请求需要多个服务交互完成，这时候就需要zookeeper 注册中心，做服务的管理和统计， 让各个服务能够及时的响应和并保持一致性。但是很简单的一个问题， 贪多就不能精。 在这个找实习的节骨眼上， 做分布式明显不是自己现阶段的主要矛盾。
2.  回调之前一直不理解， c++回调基本全部用bind+ functional 去代替了， 因为普通函数回调限制较多不能携带使用类变量。 回调贯穿了整个程序oop的解耦操作中， 当一个函数什么时候发生和发生时候怎么做不再一起，就要事先设置回调。 供其他程序调用发生时候启动。（其实可以写到其他程序那部分， 但是不能解耦。）

3. 线程数量和cpu核数量请保持一致， 这样避免不同核时候， 线程调用上下文调用过慢。 

4. 公网链接的时候， 服务器绑定自己内网网卡的地址， 客户端访问服务器所连接的公网地址， 请求过来之后自然会通过路由找到内网所在的网卡。建立链接。 
5. 如何设计开源文件的目录， bin放可执行文件  ， lib是生成的库文件 ， include是头文件， src 是源码， build 项目编译时候产生的临时文件， test放的代码， cmakelist.txt 设计编译文件的规则,  autobuild.sh 自动编译， readme.md 。  
6. 


# 参考文献
1. https://blog.csdn.net/QIANGWEIYUAN/article/details/89023980
2. 服务器编程教程

# 版本记录