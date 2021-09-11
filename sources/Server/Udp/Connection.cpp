#include <pch.hpp>
#include <Server/Udp/Connection.hpp>
#include <Udp/Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::udp::Connection::Connection(
    const int port
)
    : m_socket{ m_ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
{}

::udp::Connection::~Connection() = default;



// ------------------------------------------------------------------ send

void ::udp::Connection::send(
    const ::udp::APacket& message,
    ::boost::asio::ip::udp::endpoint endpoint
)
{
    // display the message sent
    message.display("->");

    // actual reply
    m_socket.async_send_to(
        ::boost::asio::buffer(&message, message.getSize()),
        endpoint,
        ::boost::bind(
            &Connection::startReceiveImpl,
            this
        )
    );
}

void ::udp::Connection::reply(
    const ::udp::APacket& message
)
{
    this->send(message, m_lastSenderEndpoint);
}



// ------------------------------------------------------------------ receive

void ::udp::Connection::startReceive(
    ::std::function<void(::udp::APacket&)>&& func
)
{
    if (m_userReceiveFunc) {
        throw ::std::runtime_error(::std::string(__FUNCTION__) + " has already been call");
    }
    m_userReceiveFunc = ::std::make_unique<::std::function<void(::udp::APacket&)>>(::std::move(func));
    this->startReceiveImpl();
    m_ioContext.run();
}

void ::udp::Connection::startReceiveImpl()
{
    // actual receive
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_lastSenderEndpoint,
        ::boost::bind(
            &Connection::prehandleReceive,
            this
        )
    );
}

void ::udp::Connection::prehandleReceive()
{
    // display the received message
    reinterpret_cast<::udp::APacket*>(&m_buffer)->display("<-");

    // special interactions
    if (reinterpret_cast<::udp::APacket*>(&m_buffer)->getType() == ::udp::APacket::Header::Type::ping) {
        this->reply(*reinterpret_cast<udp::packet::APacket*>(&m_buffer));
    }

    // user defined behaviours
    (*m_userReceiveFunc)(*reinterpret_cast<udp::packet::Error*>(&m_buffer));

    // special interaction
    this->startReceiveImpl();
}
