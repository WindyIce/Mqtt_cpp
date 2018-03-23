#pragma once

#include<iostream>
#include<boost\asio.hpp>
#include<boost\bind.hpp>


// 异步服务器
class Server {
private:
	// 服务实例
	boost::asio::io_service& ios_;

	// 接收器实例
	boost::asio::ip::tcp::acceptor acceptor_;

	// socket智能指针
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

public:

	Server(boost::asio::io_service& _ios) : ios_(_ios),
		acceptor_(_ios, 
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3389)) {
		// 默认执行
		start();
	}

	// 启动网络侦听的操作入口
	void start(void) {
		// 自定义的智能指针
		socket_ptr socket(new boost::asio::ip::tcp::socket(ios_));
		// 异步侦听，若有服务连接，则自动调用Server::handler_accept函数，并将error, socket传入作为参数
		acceptor_.async_accept(*socket,
			boost::bind(&Server::accept_handler, this,
				boost::asio::placeholders::error/* 此处作为占位符 */, socket));
	}

	// 请求者响应后触发的处理器
	void accept_handler(const boost::system::error_code& _ec,socket_ptr _socket) {
		// 错误码检测
		if (_ec) {
			return;
		}
		// 打印当前连接进来的客户端
		std::cout << "Client: " << _socket->remote_endpoint().address() << std::endl;
		// 异步发送 "hello CSND_Ayo" 消息到客户端，发送成功后，自动调用Server::write_handler函数
		_socket->async_write_some(boost::asio::buffer("Hello CSND_Ayo"),
			boost::bind(&Server::write_handler,this,
				boost::asio::placeholders::error/* 此处作为占位符 */));
		// 启动新的异步监听
		start();
	}

	// 完成异步写操作后的处理器
	void write_handler(const boost::system::error_code& _ec) {
		std::cout << "Servel: send message complete." << std::endl;
	}
};