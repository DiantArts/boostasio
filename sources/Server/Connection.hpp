#pragma once

#include <Packet/APacket.hpp>

namespace packet { class Text; }



namespace server {



class Connection {

public:

    // ------------------------------------------------------------------ *structors

    Connection(
        int port
    );

    ~Connection();



    // ------------------------------------------------------------------ send

    void send(
        const ::APacket& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    void reply(
        const ::APacket& message
    );



    // ------------------------------------------------------------------ receive

    void startReceive(
        ::std::function<void(::APacket&)>&& func
    );

    void stopReceive();



private:

    void startReceiveImpl();

    void prehandleReceive();



private:

    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::socket m_socket;
    ::boost::asio::ip::udp::endpoint m_lastSenderEndpoint;

    ::std::unique_ptr<::std::function<void(::APacket&)>> m_userReceiveFunc;

    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Connection::bufferLength> m_buffer;

};



} // namespace server