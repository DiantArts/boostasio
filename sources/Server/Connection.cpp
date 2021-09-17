#include <pch.hpp>
#include <Server/Connection.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ Client Infos

auto ::server::Connection::ClientInformations::operator==(
    const ::boost::asio::ip::udp::endpoint& endpoint
) const
    -> bool
{
    return endpoint == this->endpoint;
}



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
    const ::boost::asio::ip::udp::endpoint& endpoint
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

void ::server::Connection::sendToChatRoom(
    const ::packet::Text& message,
    const ::std::string& senderName
)
{
    for (auto const& client : m_connectedClients) {
        this->send<::packet::Text>(client.endpoint, "["s + senderName + "]:"s + message.toString());
    }
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

auto ::server::Connection::getLastSenderInformations()
    -> ::server::Connection::ClientInformations&
{
    return *::std::ranges::find_if(
        m_connectedClients,
        [this](const ::server::Connection::ClientInformations& clientInformations){
            return clientInformations.endpoint == m_lastSenderEndpoint;
        }
    );
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
    ::std::size_t bytesTransferred
)
{
    { // handle errors
        if (error) {
            ::std::cout << "[ERROR] - " << error.message() << ::std::endl;
            this->startReceiveImpl();
            return;
        }
    }

    auto& message{ *reinterpret_cast<::APacket*>(&m_buffer) };

    message.display("<-");

    { // handle connection
        bool isAlreadyConnected{ false };
        for (auto const& client : m_connectedClients) {
            if (m_lastSenderEndpoint == client.endpoint) {
                isAlreadyConnected = true;
                break;
            }
        }
        if (message.getType() == ::packet::Header::Type::connectionRequest) {
            if (isAlreadyConnected) {
                this->reply<::packet::Error>(::packet::Error::Type::alreadyConnected);
            }
            m_connectedClients.emplace_back(
                m_lastSenderEndpoint, reinterpret_cast<::packet::ConnectionRequest*>(&m_buffer)->toString()
            );
            this->startReceiveImpl();
            return;
        }
        if (!isAlreadyConnected) {
            this->reply<::packet::ConnectionNeeded>();
            return;
        }
    }

    auto& senderInformations{ this->getLastSenderInformations() };

    { // handle packet loss
        if (message.getId() != senderInformations.nextPacketId) {
            ::std::cout << "packet lost" << ::std::endl;
            this->reply(::packet::Loss{ message.getId() });
            ++senderInformations.nextPacketId;
            this->startReceiveImpl();
            return;
        }
        if (senderInformations.nextPacketId == 255) {
            senderInformations.nextPacketId = 1;
        } else {
            ++senderInformations.nextPacketId;
        }
    }

    { // handle special interactions
        if (message.getType() == ::packet::Header::Type::ping) {
            this->reply(message);
        }
    }

    // user defined behaviours
    (*m_userReceiveFunc)(message);

    // start receiving again
    this->startReceiveImpl();
}
