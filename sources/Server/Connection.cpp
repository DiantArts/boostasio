#include <pch.hpp>
#include <Server/Connection.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::server::Connection::Connection(
    const int port
)
    : m_socket{ m_ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
{}

::server::Connection::~Connection() = default;



// ------------------------------------------------------------------ send

void ::server::Connection::send(
    const ::APacket& message,
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

void ::server::Connection::reply(
    const ::APacket& message
)
{
    this->send(message, m_lastSenderEndpoint);
}



// ------------------------------------------------------------------ receive

void ::server::Connection::startReceive(
    ::std::function<void(::APacket&)>&& func
)
{
    if (m_userReceiveFunc) {
        throw ::std::runtime_error(::std::string(__FUNCTION__) + " has already been call");
    }
    m_userReceiveFunc = ::std::make_unique<::std::function<void(::APacket&)>>(::std::move(func));
    this->startReceiveImpl();
    m_ioContext.run();
}

void ::server::Connection::stopReceive()
{
    m_ioContext.stop();
}

void ::server::Connection::startReceiveImpl()
{
    // actual receive
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_lastSenderEndpoint,
        ::boost::bind(
            &Connection::prehandleReceive,
            this,
            ::boost::asio::placeholders::error,
            ::boost::asio::placeholders::bytes_transferred
        )
    );
}

void ::server::Connection::prehandleReceive(
    const boost::system::error_code& error,
    size_t bytesTransferred
)
{
    if (error) {
        ::std::cout << "[ERROR] - " << error.message() << ::std::endl;
        this->startReceiveImpl();
        return;
    }

    auto& message = *reinterpret_cast<::APacket*>(&m_buffer);

    // display the received message
    message.display("<-");

    // packet loss check
    if (message.getId() != m_nextPacketId) {
        this->reply(::packet::Loss{ message.getId() });
        ++m_nextPacketId;
        return;
    } else if (message.getId() == 5) {
        this->reply(::packet::Loss{ message.getId() });
        ++m_nextPacketId;
        return;
    }
    if (m_nextPacketId == 255) {
        m_nextPacketId = 1;
    } else {
        ++m_nextPacketId;
    }

    // special interactions
    if (message.getType() == ::packet::Header::Type::ping) {
        this->reply(message);
    }

    // user defined behaviours
    (*m_userReceiveFunc)(message);

    // special interaction
    this->startReceiveImpl();
}
