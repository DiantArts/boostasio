#include <pch.hpp>
#include <Client/Client.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::client::Client::Client(
    const ::std::string& host,
    const ::std::string& port
)
    : m_connectionToServer{ host, port, 200 }
{}

::client::Client::~Client() = default;



// ------------------------------------------------------------------ run

void ::client::Client::run()
{
    ::std::cout << "\r> CLIENT START <" << ::std::endl;
    m_connectionToServer.startReceive(
        ::boost::bind(&Client::handleReceive, this, ::boost::placeholders::_1)
    );
    this->startSend();
    m_connectionToServer.stopReceive();
}



// ------------------------------------------------------------------ send

void ::client::Client::startSend()
{
    ::std::cout << "Input: " << ::std::flush;
    ::std::string str;
    ::std::getline(::std::cin, str);
    while (m_connectionToServer.isValid()) {
        if (str == "/exit") {
            m_connectionToServer.send(::packet::Exit{});
        } else if (str == "/ping") {
            ::std::cout << "Latency: " << m_connectionToServer.getLatency() << ::std::endl;
        } else {
            m_connectionToServer.send(::packet::Text{ str });
        }
        ::std::getline(::std::cin, str);
    }
}



// ------------------------------------------------------------------ receive

void ::client::Client::handleReceive(
    ::APacket& message
)
{}
