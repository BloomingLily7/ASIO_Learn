/*
同步通信客户端
1.根据对端ip和端口创建endpoint
2.创建socket连接endpoint
3.采用同步读写方式通信
*/
#include <iostream>
#include<boost/asio.hpp>
using namespace std;
using namespace boost::asio::ip;
const int  MAX_LENGTH = 1024;//客户端允许发送的最大字节长度

int main()
{
    try {
        //创建上下文环境
        boost::asio::io_context ioc;
        //构建endpoint
        tcp::endpoint remote_ep(make_address("127.0.0.1"), 10086);

        tcp::socket skt(ioc);
        boost::system::error_code error = boost::asio::error::host_not_found;
        skt.connect(remote_ep, error);

        if (error) {
            cout << "connect failed. Code is: " << error.value();
            cout << "error message is: " << error.message();
            return 0;
        }

        cout << "Enter Message:";
        char request[MAX_LENGTH];
        cin.getline(request, MAX_LENGTH);
        size_t request_length = strlen(request);
        boost::asio::write(skt, boost::asio::buffer(request, request_length));

        char reply[MAX_LENGTH];
        size_t reply_length = boost::asio::read(skt, boost::asio::buffer(reply, request_length));
        cout << "Reply is: ";
        cout.write(reply, reply_length);
        cout << "\n";

    }
    catch (exception& e)
    {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}


