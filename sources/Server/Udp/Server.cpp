#include <pch.hpp>
#include <Server/Udp/Server.hpp>
#include <Udp/Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::udp::Server::Server(
    const int port
)
    : m_clientsConnection{ port }
{}

::udp::Server::~Server() = default;



// ------------------------------------------------------------------ run

void ::udp::Server::run()
{
    ::std::cout << "> SERVER START <" << ::std::endl;
    m_clientsConnection.startReceive(
        ::boost::bind(&Server::handleReceive, this, ::boost::placeholders::_1)
    );
    ::std::cout << "> EXIT SERVER <" << ::std::endl;
}



// ------------------------------------------------------------------ receive

void ::udp::Server::handleReceive(
    ::udp::APacket& message
)
{
    switch (message.getType()) {
    case ::udp::APacket::Header::Type::text:
        message.assignNewId();
        m_clientsConnection.reply(message);
        break;
    default: break;
    }
}
