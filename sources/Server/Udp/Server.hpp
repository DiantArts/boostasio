#pragma once

namespace udp { class AMessage; }



namespace udp {



class Server {

public:

    // ------------------------------------------------------------------ *structors

    Server(
        ::boost::asio::io_context& ioContext,
        int port
    );

    ~Server();



    // ------------------------------------------------------------------ methods

    void reply(
        const ::udp::AMessage& message
    );

    void startReceive(
        const boost::system::error_code& error,
        const ::std::size_t bytesSent
    );

    void handleReceive(
        const boost::system::error_code& error,
        const ::std::size_t bytesReceived
    );



private:

    ::boost::asio::io_context& m_ioContext;
    ::boost::asio::ip::udp::socket m_socket;
    ::boost::asio::ip::udp::endpoint m_senderEndpoint;
    static inline constexpr ::std::size_t maxLength{ 254 };
    ::std::array<::std::byte, Server::maxLength> m_buffer;

};



} // namespace udp
