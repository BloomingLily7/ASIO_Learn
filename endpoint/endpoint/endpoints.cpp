#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>
using namespace boost;

//�ͻ��˶˵㴴��
int client_end_point() {
	std::string raw_ip_address = "127.4.8.1";
	unsigned short port_num = 3333;
	boost::system::error_code ec;
	asio::ip::address ip_address = asio::ip::make_address(raw_ip_address, ec);

	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}

	asio::ip::tcp::endpoint ep(ip_address, port_num);//��ip��ַ�Ͷ˿ں�ת��Ϊ�˵�����ͨ��
	return 0;
}

//����˶˵㴴��
int server_end_point() {
	unsigned short port_num = 3333;
	asio::ip::address ip_address = asio::ip::address_v6::any();//�����ѡȡ������һip����ͨ��
	asio::ip::tcp::endpoint ep(ip_address, port_num);
	return 0;
}

//����ͨ��socket
int create_tcp_socket() {
	asio::io_context ioc;										//asio��socketͨ����Ҫ������
	asio::ip::tcp protocal = asio::ip::tcp::v4();		//ѡ��Э��
	asio::ip::tcp::socket skt(ioc);							//����socket
	boost::system::error_code ec;
	skt.open(protocal, ec);									//��socket
	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}
	return 0;
}

//���������acceptor��socket(old version)
int create_acceptor_socket1() {
	asio::io_context ioc;											//asio��socketͨ����Ҫ������
	asio::ip::tcp protocal = asio::ip::tcp::v6();			//ѡ��Э��
	asio::ip::tcp::acceptor acceptor(ioc);				//����socket
	boost::system::error_code ec;
	acceptor.open(protocal, ec);							//��socket
	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ".Message is " << ec.message();
		return ec.value();
	}
	return 0;
}

//��������acceptor��ip�Ͷ˿�(old version)
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

//���������acceptor��socket(new version)�����Զ�bind
int create_acceptor_socket2() {
	asio::io_context ioc;//asio��socketͨ����Ҫ������
	asio::ip::tcp::acceptor acceptor(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 3333));
	return 0;
}

//�ͻ��������������
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

//�ͻ���ͨ�����������������
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

//��������������
int accept_new_connect() {
	const int BACKLOG_SIZE = 30;//�������еĴ�С
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_context ioc;
	try {
		asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());
		acceptor.bind(ep);
		acceptor.listen(BACKLOG_SIZE);
		asio::ip::tcp::socket skt(ioc);//���socket���ںͿͻ���ͨ��
		acceptor.accept(skt);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code =" << e.code() << ". Message: " << e.what();
		return e.code().value();
	}
	return 0;
}


//buffer���ǽ��պͷ�������ʱ�������ݵĽṹ
//asio�����buffer()�������ú������ն�����ʽ���ֽ���

//��string���͵�����תΪbuffer�ܹ����ܵ�����
void use_buffer_str() {
	std::string buf = "hello world";
	asio::const_buffer output = asio::buffer(buf);
}

//������תΪbuffer�ܹ����ܵ�����
void use_buffer_array() {
	const size_t BUF_SIZE_BYTES = 20;//�������Ĵ�СΪ 20 �ֽ�
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]); //����ָ�����һ����̬������ַ�����
	auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES); //buf.get() ��ȡ����ָ������ԭʼָ��
																																//tatic_cast<void*> ���ַ�ָ��ת��Ϊ void ָ��
																																//asio::buffer() ����һ�� ASIO ���������󣬵�һ��������ָ���ڴ���ָ�룬�ڶ����������ڴ��Ĵ�С
}