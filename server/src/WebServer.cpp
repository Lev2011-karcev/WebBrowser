#include <WebServer.h>
#include <fstream>
#include <sstream>
#include <iostream>


//Инициализация all компонент
HttpsWebServer::HttpsWebServer(
	unsigned short port,
	const std::string& web_root,
	const std::string& cert_file,
	const std::string& private_key_file,
	const std::string& dh_file)
	: port_(port),
	web_root_(web_root),
	cert_file_(cert_file),
	private_key_file_(private_key_file),
	dh_file_(dh_file),
	running_(false),
	ssl_context_(ssl::context::tlsv12),
	acceptor_(io_context_,
		tcp::endpoint(tcp::v4(), port)) {
	setup_ssl_context();
	log("Сервер инициализировался на порту: " + std::to_string(port));
}

//Деструктор остановка до слободы (безопасная)

HttpsWebServer::~HttpsWebServer() {
	stop();
	log("Сервер остановлен");
}


void HttpsWebServer::setup_ssl_context() {
	try {
		ssl_context_.set_options(
			ssl::context::default_workarounds |
			ssl::context::no_sslv2 |
			ssl::context::single_dh_use);
		ssl_context_.use_certificate_chain_file(cert_file_);
		ssl_context_.use_private_key_file(private_key_file_, ssl::context::pem);
		ssl_context_.use_tmp_dh_file(dh_file_);

		log("SSL контекст конфигурирован положительно");
	}
	catch (const std::exception& e) {
		log("SSL Error: " + std::string(e.what()));
		throw;
	}
}


void HttpsWebServer::start() {
	if (running_) return;

	running_ = true;
	start_accept();

	server_thread_ = std::thread([this]() {
		log("Server thread started");
		io_context_.run();
		log("Server thread stopped");
	});

	log("Server started on https://localhost: " + std::to_string(port_);
}

void HttpsWebServer::start_accept() {
	auto socket = ssl::stream<tcp::socket>(io_context_, ssl_context_);

	acceptor_.async_accept(
		socket.lowest_layer(),
		[this, socket = std::move(socket)](const error_code& ec) mutable {
			handle_accept(std::move(socket), ec);
		});
}


void HttpsWebServer::handle_accept(ssl::stream<tcp::socket> socket, const error_code& ec) {
	if (!ec) {
		std::make_shared<HttpsSession>(std::move(socket), web_root_)->start();
		log("New connection accepted");
	}
	else {
		log("Accept error: " + ec.message());
	}
	if (running_) {
		start_accept();
	}
}
void HttpsWebServer::stop() {
	if (!running_) return;

	running_ = false;
	io_context_.stop();

	if (server_thread_.joinable()) {
		server_thread_.join();
	}


	log("Server stopped");
}

void HttpsWebServer::HttpsSession::start() {
	handshake();
}
void HttpsWebServer::HttpsSession::handshake() {
	auto self(shared_from_this());

	socket_.async_handshake(
		ssl::stream_base::server,
		[this, self](const error_code& ec) {
			if (!ec) {
				read_request();
			}
		});
}

void HttpsWebServer::HttpsSession::read_request() {
	auto self(shared_from_this());

	async_read_until(
		socket_,
		buffer_,
		"\r\n\r\n",
		[this, self](error_code ec, size_t) {
			if (!ec) {
				process_request();
			}
		});
}
void HttpsWebServer::HttpsSession::process_request() {
	std::istream stream(&buffer_);
	std::string request_line;
	std::getline(stream, request_line);

	// Парсинг HTTP-запроса
	std::istringstream iss(request_line);
	std::string method, path;
	iss >> method >> path;

	if (path.empty() || path[0] != '/') {
		path = "/index.html";
	}


	std::string full_path = web_root_ + path;

	if (full_path.find("../") != std::string::npos) {
		send_response("HTTP/1.1 403 Forbidden\r\n\r\n");
		return;
	}

	std::ifstream file(full_path, std::ios::binary);
		if (file) {
			std::string content((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
				std::string response =
				"HTTP/1.1 200 OK\r\n"
				"Content-Length: " + std::to_string(content.size()) + "\r\n"
				"\r\n" + content;
			send_response(response);
		}
		else {
			send_response("HTTP/1.1 404 Not Found\r\n\r\n");
		}
}

void HttpsWebServer::HttpsSession::send_response(const std::string& response) {
	auto self(shared_from_this());

	async_write(
		socket_,
		boost::asio::buffer(response),
		[this, self](error_code ec, size_t) {
			if (!ec) {
				// Корректное закрытие соединения
				boost::system::error_code shutdown_ec;
				socket_.shutdown(shutdown_ec);
			}
		});
}
void HttpsWebServer::log(const std::string& message) const {
	if (logger_) {
		logger_(message);
	}
	else {
		// Дефолтный вывод в консоль
		std::cout << "[SERVER] " << message << std::endl;
	}
}
void HttpsWebServer::set_logger(Logger logger) {
	logger_ = logger;
}

bool HttpsWebServer::is_running() const {
	return running_;
}
