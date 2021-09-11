#pragma once

#include <Client/Connection.hpp>


namespace packet { class Text; }



namespace client {



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
        ::client::Connection::PacketType& message
    );



private:

    ::client::Connection m_connectionToServer;

};



} // namespace client

using Client = ::client::Client;
