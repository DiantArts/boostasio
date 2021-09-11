#pragma once

#include <Udp/Packet/APacket.hpp>

namespace udp::packet { class Text; }



namespace udp {



class Connection {

public:

    // ------------------------------------------------------------------ *structors

    Connection(
        int port
    );

    ~Connection();



    // ------------------------------------------------------------------ send

    void send(
        const ::udp::APacket& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    void reply(
        const ::udp::APacket& message
    );



    // ------------------------------------------------------------------ receive

    void startReceive(
        ::std::function<void(::udp::APacket&)>&& func
    );



private:

    void startReceiveImpl();

    void prehandleReceive();



private:

    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::socket m_socket;
    ::boost::asio::ip::udp::endpoint m_lastSenderEndpoint;

    ::std::unique_ptr<::std::function<void(::udp::APacket&)>> m_userReceiveFunc;

    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Connection::bufferLength> m_buffer;

};



} // namespace udp
