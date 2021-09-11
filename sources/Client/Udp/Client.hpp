#pragma once

#include <Client/Udp/Connection.hpp>


namespace udp::packet { class Text; }



namespace udp {



class Client {

public:

    // ------------------------------------------------------------------ *structors

    Client(
        const ::std::string& host,
        const ::std::string& port
    );

    ~Client();



    // ------------------------------------------------------------------ run

    void run();



    // ------------------------------------------------------------------ communication

    void startSend();

    void handleReceive(
        ::udp::Connection::PacketType& message
    );



private:

    ::udp::Connection m_connectionToServer;

};



} // namespace udp
