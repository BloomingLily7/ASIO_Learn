/*
* 同步通信服务端
*   服务器启动，监听指定端口
    当新客户端连接时：
    创建新socket
    创建新线程处理该连接
    线程中循环读取客户端数据并回传
    客户端断开连接时，线程结束
    服务器持续运行直到被手动终止
*/
#include <iostream>
#include <boost/asio.hpp>
#include <set>
using  boost::asio::ip::tcp;
using namespace std;
const int MAX_LENGTH = 1024;

//使用shared_ptr管理TCP socket对象
typedef std::shared_ptr<tcp::socket> skt_ptr;

//使用set集合管理所有客户端线程
std::set<std::shared_ptr<std::thread>> thread_set;

void session(skt_ptr skt)
{
    try {
        for (;;) {
            char data[MAX_LENGTH];
            memset(data, '\0', MAX_LENGTH);
            boost::system::error_code error;
            size_t length = skt->read_some(boost::asio::buffer(data, MAX_LENGTH), error);

            if (error == boost::asio::error::eof) {
                cout << "connect is closed by peer" << endl;
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }
            cout << "receive from" << skt->remote_endpoint().address().to_string() << endl;
            cout << "recevice message is" << data << endl;

            boost::asio::write(*skt, boost::asio::buffer(data, length));
        }
    }
    catch (exception& e)
    {
        cerr << "Exception in thread: " << e.what() << endl;
    }
}

void server(boost::asio::io_context& ioc,unsigned short port) {
    tcp::acceptor a(ioc, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        skt_ptr skt(new tcp::socket(ioc));
        a.accept(*skt);
        auto t = std::make_shared<std::thread>(session, skt);
        thread_set.insert(t);
    }
}


int main()
{
    try{
        boost::asio::io_context ioc;
        server(ioc, 10086);
        for (auto& t : thread_set)
        {
            t->join();
        }
    }
    catch (exception& e)
    {
        cerr << "Exception is" << e.what() << endl;
    }
    return 0;
}

