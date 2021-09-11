#include <pch.hpp>
#include <Client/Udp/Client.hpp>
#include <Udp/Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::udp::Client::Client(
    const ::std::string& host,
    const ::std::string& port
)
    : m_connectionToServer{ host, port, 200 }
{}

::udp::Client::~Client() = default;



// ------------------------------------------------------------------ run

void ::udp::Client::run()
{
    ::std::cout << "\r> CLIENT START <" << ::std::endl;
    m_connectionToServer.startReceive(
        ::boost::bind(&Client::handleReceive, this, ::boost::placeholders::_1)
    );
    this->startSend();
    m_connectionToServer.stopReceive();
}



// ------------------------------------------------------------------ send

void ::udp::Client::startSend()
{
    ::std::cout << "Input: " << ::std::flush;
    ::std::string str;
    ::std::getline(::std::cin, str);
    while (m_connectionToServer.isValid()) {
        if (str == "/exit") {
            m_connectionToServer.send(::udp::packet::Exit{});
        } else if (str == "/ping") {
            ::std::cout << "Latency: " << m_connectionToServer.getLatency() << ::std::endl;
        } else {
            m_connectionToServer.send(::udp::packet::Text{ str });
        }
        ::std::getline(::std::cin, str);
    }
}



// ------------------------------------------------------------------ receive

void ::udp::Client::handleReceive(
    ::udp::APacket& message
)
{}
