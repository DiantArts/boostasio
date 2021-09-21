#pragma once

#include <Server/Room.hpp>



namespace server {



class Server {

public:

    // ------------------------------------------------------------------ *structors

    Server(
        int port
    );

    ~Server();



    // ------------------------------------------------------------------ run

    void run();



    // ------------------------------------------------------------------ receive

    void handleReceive(
        ::APacket& message
    );



private:

    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::socket m_socket;
    ::server::Room m_rooms;
    // ::std::vector<Buffer::ClientInformations> m_connectedClients; // TODO: multi rooms

};



} // namespace server

using Server = ::server::Server;
