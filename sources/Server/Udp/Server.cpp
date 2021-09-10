#include <pch.hpp>
#include <Server/Udp/Server.hpp>
#include <Udp/Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::udp::Server::Server(
    const int port
)
    : m_socket{ m_ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
{
    this->startReceive();
}

::udp::Server::~Server() = default;



// ------------------------------------------------------------------ run

void ::udp::Server::run()
{
    ::std::cout << "> SERVER START <" << ::std::endl;
    m_ioContext.run();
    ::std::cout << "> EXIT SERVER <" << ::std::endl;
}



// ------------------------------------------------------------------ send

void ::udp::Server::send(
    const ::udp::packet::Text& message,
    ::boost::asio::ip::udp::endpoint endpoint
)
{
    // actual reply
    m_socket.async_send_to(
        ::boost::asio::buffer(&message, message.getSize()),
        endpoint,
        ::boost::bind(
            &Server::startReceive,
            this
        )
    );

    // display the message sent
    message.display("->");
}

void ::udp::Server::send(
    const ::udp::APacket& message,
    ::boost::asio::ip::udp::endpoint endpoint
)
{
    // actual reply
    m_socket.async_send_to(
        ::boost::asio::buffer(&message, message.getSize()),
        endpoint,
        ::boost::bind(
            &Server::startReceive,
            this
        )
    );

    // display the message sent
    message.display("->");
}

void ::udp::Server::reply(
    const ::udp::APacket& message
)
{
    this->send(message, m_senderEndpoint);
}



// ------------------------------------------------------------------ receive

void ::udp::Server::startReceive()
{
    // actual receive
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_senderEndpoint,
        ::boost::bind(
            &Server::handleReceive,
            this
        )
    );
}

void ::udp::Server::handleReceive()
{
    // display the received message
    reinterpret_cast<::udp::APacket*>(&m_buffer)->display("<-");

    // special interactions
    switch (reinterpret_cast<::udp::APacket*>(&m_buffer)->getType()) {
    case ::udp::APacket::Header::Type::text:
        {
            auto& text{ *reinterpret_cast<udp::packet::Text*>(&m_buffer) };
            text.assignNewId();
            this->reply(text);
            break;
        }
    default:
        break;
    }

    // special interaction
    this->startReceive();
}
