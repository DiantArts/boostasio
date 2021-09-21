#pragma once

#include <Packet/APacket.hpp>

namespace packet{ class Text; }
namespace server{ class Server; }



namespace server {



class Buffer {

public:

    // ------------------------------------------------------------------ *structors

    Buffer(
        ::boost::asio::ip::udp::socket& socket
    );

    ~Buffer();



    // ------------------------------------------------------------------ send

    template <
        typename MessageType
    > void sendTo(
        const ::boost::asio::ip::udp::endpoint& endpoint,
        auto&&... args
    );

    template <
        typename MessageType
    > void sendTo(
        const ::boost::asio::ip::udp::endpoint& endpoint,
        const MessageType& message
    );

    template <
        typename MessageType
    > void sendTo(
        const ::boost::asio::ip::udp::endpoint& endpoint,
        MessageType&& message
    );



    template <
        typename MessageType
    > void reply(
        auto&&... args
    );

    template <
        typename MessageType
    > void reply(
        const MessageType& message
    );

    template <
        typename MessageType
    > void reply(
        MessageType&& message
    );



    // ------------------------------------------------------------------ receive

    template <
        void(::server::Server::*function)(::APacket& message)
    > void startReceive(
        ::server::Server& server
    );

    void stopReceive();



protected:

    // ------------------------------------------------------------------ routineHandler

    // checking routine. if fails, nothing have to be done
    virtual auto handleReceiveRoutine(
        const boost::system::error_code& error,
        ::std::size_t bytesTransferred
    ) -> bool
        = 0;



private:

    // ------------------------------------------------------------------ receive

    template <
        void(::server::Server::*function)(::APacket& message)
    > void prehandleReceive(
        ::server::Server& server,
        const boost::system::error_code& errorcode,
        ::std::size_t bytesTransferred
    );



protected:

    // ------------------------------------------------------------------ connection
    ::boost::asio::ip::udp::socket& m_socket;
    ::boost::asio::ip::udp::endpoint m_lastSenderEndpoint;

    // ------------------------------------------------------------------ buffer
    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Buffer::bufferLength> m_buffer;

};



} // namespace server

#include <Server/Buffer.impl.hpp>
