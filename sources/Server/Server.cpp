#include <pch.hpp>
#include <Server/Server.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::server::Server::Server(
    const int port
)
    : m_clientsConnection{ port }
{}

::server::Server::~Server() = default;



// ------------------------------------------------------------------ run

void ::server::Server::run()
{
    ::std::cout << "> SERVER START <" << ::std::endl;
    m_clientsConnection.startReceive(
        ::boost::bind(&Server::handleReceive, this, ::boost::placeholders::_1)
    );
    ::std::cout << "> EXIT SERVER <" << ::std::endl;
}



// ------------------------------------------------------------------ receive

void ::server::Server::handleReceive(
    ::APacket& message
)
{
    switch (message.getType()) {
    case ::APacket::Header::Type::text:
        message.assignNewId();
        m_clientsConnection.reply(message);
        break;
    default: break;
    }
}
