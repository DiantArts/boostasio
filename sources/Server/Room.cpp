#include <pch.hpp>
#include <Server/Room.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ Client Infos

auto ::server::Room::ClientInformations::operator==(
    const ::boost::asio::ip::udp::endpoint& endpoint
) const
    -> bool
{
    return endpoint == this->endpoint;
}



// ------------------------------------------------------------------ *structors

server::Room::Room(
    ::boost::asio::ip::udp::socket& socket
)
    : ::server::Buffer{ socket }
{}

server::Room::~Room() = default;



// ------------------------------------------------------------------ get

auto ::server::Room::get(
    const ::boost::asio::ip::udp::endpoint& clientEndpoint
) const
    -> Room::ClientInformations
{
    auto it{
        ::std::ranges::find_if(
            m_clients,
            [
                &clientEndpoint
            ](
                const Room::ClientInformations& clientInformations
            ){
                return clientInformations.endpoint == clientEndpoint;
            }
        )
    };
    if (it == m_clients.end()) {
        throw ::std::runtime_error("trying to get a client not connected to the room");
    }
    return *it;
}

auto ::server::Room::operator[](
    const ::boost::asio::ip::udp::endpoint& clientEndpoint
) const
    -> Room::ClientInformations
{
    return this->get(clientEndpoint);
}



auto ::server::Room::begin()
    -> Room::ContainerType::iterator
{
    return m_clients.begin();
}

auto ::server::Room::end()
    -> Room::ContainerType::iterator
{
    return m_clients.end();
}

auto ::server::Room::begin() const
    -> Room::ContainerType::const_iterator
{
    return m_clients.begin();
}

auto ::server::Room::end() const
    -> Room::ContainerType::const_iterator
{
    return m_clients.end();
}

auto ::server::Room::cbegin() const
    -> Room::ContainerType::const_iterator
{
    return m_clients.cbegin();
}

auto ::server::Room::cend() const
    -> Room::ContainerType::const_iterator
{
    return m_clients.cend();
}



// ------------------------------------------------------------------ informations

auto ::server::Room::isConnected(
    const ::boost::asio::ip::udp::endpoint& clientEndpoint
) const
    -> bool
{
    auto it{
        ::std::ranges::find_if(
            m_clients,
            [
                &clientEndpoint
            ](
                const ::server::Room::ClientInformations& clientInformations
            ){
                return clientInformations.endpoint == clientEndpoint;
            }
        )
    };
    return it != m_clients.end();
}



// ------------------------------------------------------------------ handle things

auto ::server::Room::handleErrors(
    const boost::system::error_code& error,
    ::std::size_t bytesTransferred
) -> bool
{
    auto& message{ *reinterpret_cast<::APacket*>(&m_buffer) };
    if (error) {
        ::std::cout << "[ERROR] - " << error.message() << ::std::endl;
        return false;
    }
    message.display("<-");
    return true;
}

auto ::server::Room::handleConnection()
    -> bool
{
    auto& request{ *reinterpret_cast<::APacket*>(&m_buffer) };
    auto isConnected{ this->isConnected(m_lastSenderEndpoint) };

    if (request.getType() == ::packet::Header::Type::connectionRequest) {
        if (isConnected) {
            this->sendTo<::packet::Error>(m_lastSenderEndpoint, ::packet::Error::Type::alreadyConnected);
            return false;
        } else {
            m_clients.emplace_back(
                m_lastSenderEndpoint, reinterpret_cast<::packet::ConnectionRequest*>(&request)->toString()
            );
            return false;
        }
    }
    if (!isConnected) {
        this->reply<::packet::ConnectionNeeded>();
        return false;
    }
    return true;
}

auto ::server::Room::handlePacketLoss()
    -> bool
{
    auto& message{ *reinterpret_cast<::APacket*>(&m_buffer) };
    auto senderInformations{ this->get(m_lastSenderEndpoint) };
    if (message.getId() != senderInformations.nextPacketId) {
        this->sendTo<::packet::Loss>(m_lastSenderEndpoint, message.getId());
        ++senderInformations.nextPacketId;
        return false;
    }
    if (senderInformations.nextPacketId == 255) {
        senderInformations.nextPacketId = 1;
    } else {
        ++senderInformations.nextPacketId;
    }
    return true;
}

auto ::server::Room::handleReceiveRoutine(
    const boost::system::error_code& error,
    ::std::size_t bytesTransferred
) -> bool
{
    return
        this->handleErrors(error, bytesTransferred) ||
        this->handleConnection() ||
        this->handlePacketLoss();
}
