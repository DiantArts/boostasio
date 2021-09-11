#pragma once

#include <Server/Connection.hpp>



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

    ::server::Connection m_clientsConnection;

};



} // namespace server

using Server = ::server::Server;
