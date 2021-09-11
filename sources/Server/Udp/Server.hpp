#pragma once

#include <Server/Udp/Connection.hpp>

namespace udp::packet { class Text; }



namespace udp {



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
        ::udp::APacket& message
    );



private:

    ::udp::Connection m_clientsConnection;

};



} // namespace udp
