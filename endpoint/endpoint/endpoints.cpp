#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>
using namespace boost;

//客户端端点创建
int client_end_point() {
	std::string raw_ip_address = "127.4.8.1";
	unsigned short port_num = 3333;
	boost::system::error_code ec;
	asio::ip::address ip_address = asio::ip::make_address(raw_ip_address, ec);

	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}

	asio::ip::tcp::endpoint ep(ip_address, port_num);//把ip地址和端口号转换为端点用于通信
	return 0;
}

//服务端端点创建
int server_end_point() {
	unsigned short port_num = 3333;
	asio::ip::address ip_address = asio::ip::address_v6::any();//服务端选取本地任一ip进行通信
	asio::ip::tcp::endpoint ep(ip_address, port_num);
	return 0;
}

//创建通信socket
int create_tcp_socket() {
	asio::io_context ioc;										//asio的socket通信需要上下文
	asio::ip::tcp protocal = asio::ip::tcp::v4();		//选择协议
	asio::ip::tcp::socket skt(ioc);							//生成socket
	boost::system::error_code ec;
	skt.open(protocal, ec);									//打开socket
	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}
	return 0;
}

//创建服务端acceptor的socket(old version)
int create_acceptor_socket1() {
	asio::io_context ioc;											//asio的socket通信需要上下文
	asio::ip::tcp protocal = asio::ip::tcp::v6();			//选择协议
	asio::ip::tcp::acceptor acceptor(ioc);				//生成socket
	boost::system::error_code ec;
	acceptor.open(protocal, ec);							//打开socket
	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}
	return 0;
}

//服务器端acceptor绑定ip和端口(old version)
int bind_acceptor_socket() {
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_context ioc;
	asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());
	boost::system::error_code ec;
	acceptor.bind(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}
	return 0;
}

//创建服务端acceptor的socket(new version)，已自动bind
int create_acceptor_socket2() {
	asio::io_context ioc;//asio的socket通信需要上下文
	asio::ip::tcp::acceptor acceptor(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 3333));
	return 0;
}

//客户端与服务器连接
int connect_to_end() {
	std::string raw_ip_address = "192.168.1.124";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::make_address(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket skt(ioc, ep.protocol());
		skt.connect(ep);
	}
	catch(system::system_error& e){
		std::cout << "Error occured! Error code =" << e.code() << ". Message: " << e.what();
		return e.code().value();
	}
	return 0;
}

//客户端通过域名与服务器连接
int dns_connect_to_end() {
	std::string host = "bilibili.com";
	std::string port_num = "3333";
	asio::io_context ioc;
	asio::ip::tcp::resolver resolver(ioc);
	//asio::ip::tcp::resolver::results_type it = resolver.resolve(host, port_num, asio::ip::resolver_base::numeric_host);
	try {
		asio::ip::tcp::resolver::results_type it = resolver.resolve(host, port_num, asio::ip::resolver_base::numeric_host);
		asio::ip::tcp::socket skt(ioc);
		asio::connect(skt, it);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code =" << e.code() << ". Message: " << e.what();
		return e.code().value();
	}
	return 0;
}

//服务器接受连接
int accept_new_connect() {
	const int BACKLOG_SIZE = 30;//监听队列的大小
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_context ioc;
	try {
		asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());
		acceptor.bind(ep);
		acceptor.listen(BACKLOG_SIZE);
		asio::ip::tcp::socket skt(ioc);//这个socket用于和客户端通信
		acceptor.accept(skt);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code =" << e.code() << ". Message: " << e.what();
		return e.code().value();
	}
	return 0;
}


//buffer就是接收和发送数据时缓存数据的结构
//asio提出了buffer()函数，该函数接收多种形式的字节流

//将string类型的数据转为buffer能够接受的类型
void use_buffer_str() {
	std::string buf = "hello world";
	asio::const_buffer output = asio::buffer(buf);
}

//将数组转为buffer能够接受的类型
void use_buffer_array() {
	const size_t BUF_SIZE_BYTES = 20;//缓冲区的大小为 20 字节
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]); //智能指针管理一个动态分配的字符数组
	auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES); //buf.get() 获取智能指针管理的原始指针
																																//tatic_cast<void*> 将字符指针转换为 void 指针
																																//asio::buffer() 创建一个 ASIO 缓冲区对象，第一个参数是指向内存块的指针，第二个参数是内存块的大小
}