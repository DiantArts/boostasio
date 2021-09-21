#include <pch.hpp>
#include <Server/Server.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::server::Server::Server(
    const int port
)
    : m_socket{ m_ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
    , m_rooms{ m_socket }
{}

::server::Server::~Server() = default;



// ------------------------------------------------------------------ run

void ::server::Server::run()
{
    ::std::cout << "> SERVER START <" << ::std::endl;

    m_rooms.startReceive<&Server::handleReceive>(*this);
    m_ioContext.run();
    ::std::cout << "> EXIT SERVER <" << ::std::endl;
}



// ------------------------------------------------------------------ receive

void ::server::Server::handleReceive(
    ::APacket& message
)
{
    switch (message.getType()) {
    case ::packet::Header::Type::text:
        message.assignNewId();
        // TODO: send to chat the all room
        // m_rooms.send(
            // reinterpret_cast<::packet::Text&>(message), m_rooms.getLastSenderInformations().name
        // );
        break;
    default: break;
    }
}
