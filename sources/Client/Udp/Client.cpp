#include <pch.hpp>
#include <Client/Udp/Client.hpp>
#include <Udp/Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::udp::Client::Client(
    const ::std::string& host,
    const ::std::string& port
)
    : m_endpoint{
        ::boost::asio::ip::address::from_string(host),
        static_cast<short unsigned int>(::std::atoi(port.c_str()))
    }
    , m_socket{ m_ioContext }
{
    m_socket.open(::boost::asio::ip::udp::v4());
}

::udp::Client::~Client() = default;



// ------------------------------------------------------------------ run

void ::udp::Client::run()
{
    ::std::thread senderThread{ &Client::startSend, this };
    this->startReceive();
    ::std::cout << "> CLIENT START <" << ::std::endl;
    m_ioContext.run();
    ::std::cout << "> EXIT CLIENT <" << ::std::endl;
    senderThread.join();
}



// ------------------------------------------------------------------ send

void ::udp::Client::startSend()
{
    while (true) {
        ::std::cout << "Input: " << ::std::flush;
        ::std::string str;
        ::std::getline(::std::cin, str);
        if (str == "/exit") {
            this->handleSend(::udp::packet::Exit{});
        }
        ::udp::packet::Text message{ str };
        char* data{ (char*)&message };
        this->handleSend(message);
    }
}

void ::udp::Client::handleSend(
    const ::udp::APacket& message
)
{
    message.display("->");
    m_socket.send_to(
        ::boost::asio::buffer(&message, message.getSize()),
        m_endpoint
    );
}



// ------------------------------------------------------------------ receive

void ::udp::Client::startReceive()
{
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_endpoint,
        ::boost::bind(
            &Client::handleResponse,
            this
        )
    );
}

void ::udp::Client::handleResponse()
{
    ::std::cout << '\r';
    reinterpret_cast<::udp::APacket*>(&m_buffer)->display("<-");
    ::std::cout << "Input: " << ::std::flush;

    // special interactions
    switch (reinterpret_cast<::udp::APacket*>(&m_buffer)->getHeader().type) {
    case ::udp::APacket::Header::Type::ping:
        ::std::cout << "[System]: just received a ping" << ::std::endl;
        break;
    case ::udp::APacket::Header::Type::error:
        ::std::cout << reinterpret_cast<udp::packet::Error*>(&m_buffer)->toString() << ::std::endl;
        break;
    default: break;
    }
    this->startReceive();
}
