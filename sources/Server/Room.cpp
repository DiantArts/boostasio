#include <pch.hpp>
#include <Server/Room.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

server::Room::Room() = default;

server::Room::~Room() = default;



// ------------------------------------------------------------------ get

auto ::server::Room::get{
    const ::boost::asio::ip::udp::endpoint& clientEndpoint
} const
    -> Room::ClientInformations
{
    auto it{
        ::std::ranges::find_if(
            m_connectedClients,
            [
                &clientEndpoint
            ](
                const ::server::Connection::ClientInformations& clientInformations
            ){
                return clientInformations.endpoint == clientEndpoint;
            }
        )
    };
    if (it == m_connectedClients.end()) {
        throw ::std::runtime_error("trying to get a client not connected to the room")
    }
    return *it;
}

auto ::server::Room::operator[]{
    const ::boost::asio::ip::udp::endpoint& clientEndpoint
} const
    -> Room::ClientInformations
{
    return this->git(clientEndpoint);
}



// ------------------------------------------------------------------ informations

auto ::server::Room::isConnected(
    const ::boost::asio::ip::udp::endpoint& clientEndpoint
) const
    -> bool
{
    auto it{
        ::std::ranges::find_if(
            m_connectedClients,
            [
                &clientEndpoint
            ](
                const ::server::Connection::ClientInformations& clientInformations
            ){
                return clientInformations.endpoint == clientEndpoint;
            }
        )
    };
    return it != m_connectedClients.end();
}



// ------------------------------------------------------------------ send

void ::server::Room::send(
    const ::APacket& message
)
{
    for (auto const& client : m_connectedClients) {
        this->send<::packet::Text>(message.toString());
    }
}

void ::server::Room::operator<<(
    const ::APacket& message
)
{
    this->send(message);
}



// ------------------------------------------------------------------ handle things

auto ::server::Room::handleConnection(
    const ::boost::asio::ip::udp::endpoint& clientEndpoint,
    const ::packet& request
) -> bool
{
    auto isConnected{ this->isConnected() };

    if (message.getType() == ::packet::Header::Type::connectionRequest) {
        if (isConnected) {
            this->send<::packet::Error>(clientEndpoint, ::packet::Error::Type::alreadyConnected);
            return false;
        } else {
            m_connectedClients.emplace_back(
                m_lastSenderEndpoint, reinterpret_cast<::packet::ConnectionRequest*>(&request)->toString()
            );
            return true;
        }
    }
    if (!isConnected) {
        this->reply<::packet::ConnectionNeeded>();
        return false;
    }
    return true;
}

auto ::server::Room::handlePacketLoss(
    const ::boost::asio::ip::udp::endpoint& clientEndpoint,
    const ::packet& message
) -> bool
{
    auto& senderInformations{ this->get[clientEndpoint] };
    if (message.getId() != senderInformations.nextPacketId) {
        ::std::cout << "packet lost" << ::std::endl;
        this->send<::packet::Loss>(clientEndpoint, message.getId());
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
