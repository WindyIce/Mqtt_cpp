#pragma once

#include<iostream>
#include<boost\asio.hpp>
#include<boost\bind.hpp>


// �첽������
class Server {
private:
	// ����ʵ��
	boost::asio::io_service& ios_;

	// ������ʵ��
	boost::asio::ip::tcp::acceptor acceptor_;

	// socket����ָ��
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

public:

	Server(boost::asio::io_service& _ios) : ios_(_ios),
		acceptor_(_ios, 
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3389)) {
		// Ĭ��ִ��
		start();
	}

	// �������������Ĳ������
	void start(void) {
		// �Զ��������ָ��
		socket_ptr socket(new boost::asio::ip::tcp::socket(ios_));
		// �첽���������з������ӣ����Զ�����Server::handler_accept����������error, socket������Ϊ����
		acceptor_.async_accept(*socket,
			boost::bind(&Server::accept_handler, this,
				boost::asio::placeholders::error/* �˴���Ϊռλ�� */, socket));
	}

	// ��������Ӧ�󴥷��Ĵ�����
	void accept_handler(const boost::system::error_code& _ec,socket_ptr _socket) {
		// ��������
		if (_ec) {
			return;
		}
		// ��ӡ��ǰ���ӽ����Ŀͻ���
		std::cout << "Client: " << _socket->remote_endpoint().address() << std::endl;
		// �첽���� "hello CSND_Ayo" ��Ϣ���ͻ��ˣ����ͳɹ����Զ�����Server::write_handler����
		_socket->async_write_some(boost::asio::buffer("Hello CSND_Ayo"),
			boost::bind(&Server::write_handler,this,
				boost::asio::placeholders::error/* �˴���Ϊռλ�� */));
		// �����µ��첽����
		start();
	}

	// ����첽д������Ĵ�����
	void write_handler(const boost::system::error_code& _ec) {
		std::cout << "Servel: send message complete." << std::endl;
	}
};