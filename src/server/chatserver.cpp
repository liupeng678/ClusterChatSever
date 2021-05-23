#include "chatserver.hpp" 
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 注册链接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量
    _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 客户端断开链接
    if (!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);  // 如果服务器异常了， 业务类也要做一些关闭， 这里解耦不了啊。还是要加的
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    // 测试，添加json打印代码
    cout << buf << endl;

    // 数据的反序列化
    json js = json::parse(buf);
    //当数据来了， 直接触发对应的回调onConnection， 回调到这里解耦网络模块的代码和业务模块的代码 , 通过定义一个类， 这个类在map中事先配置js["msgid"]各种回调方法为自身的成员函数， ， 就避免了在这个类写业务代码，做到了网络层和业务层的解耦，
    // 
    // 通过js["msgid"] 获取=》业务handler函数对象=》conn  js  time
    // 将业务封装到了业务类中， 当网络这块回调启动后根据数据的id拿到业务类的对应方法进行调用。 业务再怎么改， 这边代码都不动。 
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>()); 
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}