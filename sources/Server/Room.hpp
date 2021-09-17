#pragma once

#include <Packet/APacket.hpp>

namespace packet{ class ConnectionRequest; }



namespace server {



class Room {

public:

    // ------------------------------------------------------------------ Client Infos

    // TODO: get that into another file, implement logic
    struct ClientInformations {
        ::boost::asio::ip::udp::endpoint endpoint;
        ::std::string name;
        ::std::uint8_t nextPacketId{ 1 }; // TODO: operator++

        auto operator==(
            const ::boost::asio::ip::udp::endpoint& other
        ) const
            -> bool;
    };



public:

    // ------------------------------------------------------------------ *structors

    Room();

    ~Room();



    // ------------------------------------------------------------------ get

    auto get{
        const ::boost::asio::ip::udp::endpoint& clientEndpoint
    } const
        -> Room::ClientInformations;

    auto operator[]{
        const ::boost::asio::ip::udp::endpoint& clientEndpoint
    } const
        -> Room::ClientInformations;



    // ------------------------------------------------------------------ informations

    auto isConnected(
        const ::boost::asio::ip::udp::endpoint& clientEndpoint
    ) const
        -> bool;



    // ------------------------------------------------------------------ send

    void send(
        const ::APacket& message
    );

    void operator<<(
        const ::APacket& message
    );



    // ------------------------------------------------------------------ handle things

    auto connect(
        const ::boost::asio::ip::udp::endpoint& clientEndpoint,
        const ::packet& request
    ) -> bool;




private:

    ::std::vector<Room::ClientInformations> m_connectedClients;

};



} // namespace server
