#include <pch.hpp>
#include <Server/Udp/Server.hpp>
#include <Udp/MessageTypes.hpp>



// ------------------------------------------------------------------ *structors

::udp::Server::Server(
    ::boost::asio::io_context& ioContext,
    const int port
)
    : m_ioContext{ ioContext }
    , m_socket{ m_ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
{
    m_socket.async_receive_from(
        boost::asio::buffer(m_buffer, this->maxLength),
        m_senderEndpoint,
        boost::bind(
            &Server::handleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

::udp::Server::~Server() = default;



// ------------------------------------------------------------------ methods

void ::udp::Server::reply(
    const ::udp::AMessage& message
)
{
    m_socket.async_send_to(
        boost::asio::buffer(&message, message.getHeader().size + message.getHeaderSize()),
        m_senderEndpoint,
        boost::bind(
            &Server::startReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}


void ::udp::Server::startReceive(
    const boost::system::error_code& error,
    const ::std::size_t bytesSent
)
{
    m_socket.async_receive_from(
        boost::asio::buffer(m_buffer, maxLength),
        m_senderEndpoint,
        boost::bind(
            &Server::handleReceive,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

void ::udp::Server::handleReceive(
    const boost::system::error_code& error,
    const ::std::size_t bytesReceived
)
{
    if (!error && bytesReceived > 0) {
        switch (reinterpret_cast<::udp::AMessage*>(&m_buffer)->getHeader().type) {
        case ::udp::AMessage::Header::Type::exit:
            this->reply(::udp::ping::Confirmation{});
            break;
        case ::udp::AMessage::Header::Type::latency:
            this->reply(::udp::ping::Confirmation{});
            break;
        case ::udp::AMessage::Header::Type::chatMessage:
            this->reply(*reinterpret_cast<udp::message::Chat*>(&m_buffer));
            break;
        default:
            this->reply(::udp::message::Error{ ::udp::message::Error::Type::unknown });
        }
        if (reinterpret_cast<::udp::AMessage*>(&m_buffer)->isImportant()) {
            this->reply(::udp::ping::Confirmation{});
        }
    } else {
        m_socket.async_receive_from(
            boost::asio::buffer(m_buffer, this->maxLength),
            m_senderEndpoint,
            boost::bind(
                &Server::handleReceive,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }
}
