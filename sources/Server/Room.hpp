#pragma once

#include <Packet/APacket.hpp>
#include <Server/Buffer.hpp>

namespace packet{ class ConnectionRequest; }



namespace server {



class Room
    : public server::Buffer
{

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

    using ContainerType = ::std::vector<Room::ClientInformations>;



public:

    // ------------------------------------------------------------------ *structors

    Room(
        ::boost::asio::ip::udp::socket& socket
    );

    ~Room();



    // ------------------------------------------------------------------ get

    auto get(
        const ::boost::asio::ip::udp::endpoint& clientEndpoint
    ) const
        -> Room::ClientInformations;

    auto operator[](
        const ::boost::asio::ip::udp::endpoint& clientEndpoint
    ) const
        -> Room::ClientInformations;



    auto begin()
        -> Room::ContainerType::iterator;

    auto end()
        -> Room::ContainerType::iterator;

    auto begin() const
        -> Room::ContainerType::const_iterator;

    auto end() const
        -> Room::ContainerType::const_iterator;

    auto cbegin() const
        -> Room::ContainerType::const_iterator;

    auto cend() const
        -> Room::ContainerType::const_iterator;



    // ------------------------------------------------------------------ informations

    auto isConnected(
        const ::boost::asio::ip::udp::endpoint& clientEndpoint
    ) const
        -> bool;



    // ------------------------------------------------------------------ handle things

    virtual auto handleReceiveRoutine(
        const boost::system::error_code& error,
        ::std::size_t bytesTransferred
    ) -> bool
        override;


private:

    // ------------------------------------------------------------------ handle things

    auto handleErrors(
        const boost::system::error_code& error,
        ::std::size_t bytesTransferred
    ) -> bool;

    auto handleConnection()
        -> bool;

    auto handlePacketLoss()
        -> bool;



private:

    Room::ContainerType m_clients;


};



} // namespace server
