/*
muduo网络库给用户提供了两个主要的类
TcpServer ： 用于编写服务器程序的
TcpClient ： 用于编写客户端程序的

epoll + 线程池
好处：能够把网络I/O的代码和业务代码区分开
                        用户的连接和断开       用户的可读写事件
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;  
using namespace muduo;   // 防止命名污染， 开源代码都得用namespace 
using namespace muduo::net;
using namespace placeholders;

/*基于muduo网络库开发服务器程序
1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写时间的回调函数
5.设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/
class ChatServer   // 封装ChatServer 
{
public:
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // IP+Port
               const string &nameArg)
        : _server(loop, listenAddr, nameArg), _loop(loop)  //  创建eventloop 和 tcpsever
    {
        // 给服务器注册用户连接的创建和断开回调
        //lp :  std::bind 将当前函数做成一个函数对象， 传递给function中接收保持其永久性。  当tcpsever.start 时候
        // 线程会阻塞到epoller- wait 中， 当监听的fd有连接或者读写事件发生， 程序不会再阻塞， 而是处理这个fd对应
        // 回调函数， 也就是用functional 存储的对象 +（）去做正式的调用。setConnectionCallback 就是提前设置好这个东西。

        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));  // _1参数占位

        // 设置服务器端的线程数量 1个I/O线程   3个worker线程
        _server.setThreadNum(4);   // 
    }

    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开  epoll listenfd accept ， 
    // lp : 我们怎么知道这个函数定义呢？ 首先去看一下functional 中定义回调对象如下std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
    //这告诉我们void 返回类型， TcpConnectionPtr&参数类型，  TcpConnectionPtr& 是什么呢/ 就是一个对象指针， 那边调用的时候会有这个对象指针。 
    // 这里还有一个问题， 那我们传入的this是干嘛了？ 其实只是去告诉函数的地址。
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:offline" << endl;
            conn->shutdown(); // close(fd)
            // _loop->quit();
        }
    }

    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time)               // 接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << " time:" << time.toFormattedString() << endl;
        conn->send(buf);
    }

    TcpServer _server; // #  server
    EventLoop *_loop;  // #2 epoll
};

int main()
{
    EventLoop loop; // epoll
    InetAddress addr("172.16.0.8", 80);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); // listenfd epoll_ctl=>epoll
    loop.loop();    // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等
}