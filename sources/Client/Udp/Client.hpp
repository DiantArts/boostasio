#pragma once

namespace udp { class AMessage; }



namespace udp {



class Client {

public:

    // ------------------------------------------------------------------ *structors

    Client(
        const ::std::string& host,
        const ::std::string& port
    );

    ~Client();



    // ------------------------------------------------------------------ methods

    void run();

    void send(
        const ::udp::AMessage& message
    );

    void receive();

    void handleResponse();



    // ------------------------------------------------------------------ action

    auto getServerLatency()
        -> float;

    void exit();



private:

    ::boost::asio::io_service m_ioService;
    ::boost::asio::ip::udp::resolver m_resolver;
    ::boost::asio::ip::udp::endpoint m_receiverEndpoint;
    ::boost::asio::ip::udp::socket m_socket;
    ::std::array<::std::byte, 254> m_buffer;

};



} // namespace udp
