#pragma once

#include <Udp/Packet/APacket.hpp>

namespace udp::packet { class Text; }



namespace udp {



class Server {

public:

    // ------------------------------------------------------------------ *structors

    Server(
        int port
    );

    ~Server();



    // ------------------------------------------------------------------ run

    void run();



    // ------------------------------------------------------------------ send

    void send(
        const ::udp::packet::Text& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    void send(
        const ::udp::APacket& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    void reply(
        const ::udp::APacket& message
    );



    // ------------------------------------------------------------------ receive

    void startReceive();

    void handleReceive();



private:

    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::socket m_socket;
    ::boost::asio::ip::udp::endpoint m_senderEndpoint;

    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Server::bufferLength> m_buffer;

};



} // namespace udp
