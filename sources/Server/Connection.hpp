#pragma once

#include <Packet/APacket.hpp>



namespace server {



class Connection {

public:

    // ------------------------------------------------------------------ *structors

    Connection(
        int port
    );

    ~Connection();



    // ------------------------------------------------------------------ packets

    void clearSentPackets();

    auto isPacketSent(
        ::std::uint8_t packetId
    ) -> bool;



    // ------------------------------------------------------------------ send

    template <
        typename MessageType
    > void send(
        ::boost::asio::ip::udp::endpoint endpoint,
        auto&&... args
    );

    template <
        typename MessageType
    > void send(
        ::boost::asio::ip::udp::endpoint endpoint
        MessageType&& message,
    );

    void send(
        ::boost::asio::ip::udp::endpoint endpoint,
        ::std::unique_ptr<::APacket>&& message
    );



    template <
        typename MessageType
    > void reply(
        auto&&... args
    );

    template <
        typename MessageType
    > void reply(
        MessageType&& message
    );

    void reply(
        ::std::unique_ptr<::APacket>&& message
    );



    // ------------------------------------------------------------------ receive

    template <
        auto function
    > void startReceive();

    void stopReceive();



private:

    template <
        auto function
    > void startReceiveImpl();

    template <
        auto function
    > void prehandleReceive(
        const boost::system::error_code& errorcode,
        size_t bytesTransferred
    );



private:

    // ------------------------------------------------------------------ connection
    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::endpoint m_lastSenderEndpoint;
    ::boost::asio::ip::udp::socket m_socket;

    // ------------------------------------------------------------------ receive
    ::std::unique_ptr<::std::function<void(::APacket&)>> m_userReceiveFunc;
    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Connection::bufferLength> m_buffer;

    // ------------------------------------------------------------------ tcp
    ::std::uint8_t m_nextPacketId{ 1 };

};



} // namespace server
