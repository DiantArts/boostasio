#pragma once

#include <Packet/APacket.hpp>

namespace packet{ class Text; }



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
        const ::boost::asio::ip::udp::endpoint& endpoint
    );

    template <
        typename MessageType
    > void send(
        const ::boost::asio::ip::udp::endpoint& endpoint,
        auto&&... args
    );

    void sendToChatRoom(
        const ::packet::Text& message,
        const ::std::string& senderName
    );

    void reply(
        const ::APacket& message
    );

    template <
        typename MessageType
    > void reply(
        auto&&... args
    );



    // ------------------------------------------------------------------ receive

    void startReceive(
        ::std::function<void(::APacket&)>&& func
    );

    void stopReceive();

    auto getLastSenderInformations()
        -> ::server::Connection::ClientInformations&;



private:

    void startReceiveImpl();

    void prehandleReceive(
        const boost::system::error_code& errorcode,
        ::std::size_t bytesTransferred
    );



private:

    // ------------------------------------------------------------------ connection
    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::socket m_socket;
    ::boost::asio::ip::udp::endpoint m_lastSenderEndpoint;
    ::std::vector<Connection::ClientInformations> m_connectedClients; // room

    // ------------------------------------------------------------------ receive
    ::std::unique_ptr<::std::function<void(::APacket&)>> m_userReceiveFunc;
    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Connection::bufferLength> m_buffer;

};



} // namespace server

#include <Server/Connection.impl.hpp>
