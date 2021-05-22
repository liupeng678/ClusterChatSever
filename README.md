# ClusterChatSever


# 介绍

## 背景
 秋招在急，简历中没有一个合适的项目供面试官和自己去展开提问。  而且在学习c++的过程中最大的问题就是没有合适的项目去融汇贯通， 不知道做什么才好，像java可以做很多有意思的后台项目， 但是c++貌似如果只是懂语法去做这些后台项目很难， 不知道从什么点出发去做。我想通过这个项目去掌握C++如何开发后台项目，不涉及分布式。  通过这个项目明白后台的开发流程， 让自己的c++能够做到实际开发的水平， 敢写c++。
 
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
最终实现了一个高并发的集群服务器。 如果我们还需要扩展的话， 需要将这些服务拆分，通过RPC框架注册成RPC服务， 然后供客户端请求， 有时候一个请求需要多个服务交互完成，这时候就需要zookeeper 注册中心，做服务的管理和统计， 让各个服务能够及时的响应和并保持一致性。 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522133518257.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdXBlbmcxOTk3MDExOQ==,size_16,color_FFFFFF,t_70)


 ### 3. 技术栈
 C++, linux , cmake , mysql , redis , nginx , muduo , gdb , git, json



 ### 4. 具体步骤
 
 




# 问题记录

## 编程问题

1. define 内容别的地方引用时候要在头文件里面define才能用到。 


## 设计问题


# 版本记录