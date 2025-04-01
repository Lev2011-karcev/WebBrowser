#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <memory>
#include <functional>


namespace ssl = boost::asio::ssl;
using boost::asio::ip::tcp;
using boost::system::error_code;


class HttpsWebServer {
public :

	HttpsWebServer(
		unsigned short port,
		const std::string& web_root,
		const std::string& cert_file,
		const std::string& private_key_file,
		const std::string& dh_file
	);
	~HttpsWebServer();
	void start();
	void stop();
	bool is_running() const;


	// ну типо Callback для логов
	using Logger = std::function<void(const std::string&)>;
	void set_logger(Logger logger);
private :
	class HttpsSession : public std::enable_shared_from_this<HttpsSession> {
	public :
        HttpsSession(ssl::stream<tcp::socket> socket, const std::string& web_root):
            socket_(std::move(socket)),  // Перемещаем сокет в класс
            web_root_(web_root) {}
			void start();
    private :
		void handshake();
		void read_request();
		void process_request();
		void send_responce(const std::string& response);

		ssl::stream<tcp::socket> socket_;
		boost::asio::streambuf buffer_;
		std::string web_root_;
	};

	void start_accept();
	void handle_accept(ssl::stream<tcp::socket> socket, const error_code& ec);
	void setup_ssl_context();
	void log(const std::string& message);

	unsigned short port_;
	std::string web_root_;
	std::string cert_file_;
	std::string private_key_file_;
	std::string dh_file_;
	bool running_;

	boost::asio::io_context io_context_;
	ssl::context ssl_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::thread server_thread_;


	Logger logger_;
};
