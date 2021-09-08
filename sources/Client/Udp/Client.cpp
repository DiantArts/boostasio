#include <pch.hpp>
#include <Client/Udp/Client.hpp>
#include <Udp/MessageTypes.hpp>



// ------------------------------------------------------------------ *structors

::udp::Client::Client(
    const ::std::string& host,
    const ::std::string& port
)
    : m_resolver{ m_ioService }
    , m_receiverEndpoint{ *m_resolver.resolve(::boost::asio::ip::udp::v4(), host, port) }
    , m_socket{ m_ioService }
{
    m_socket.open(::boost::asio::ip::udp::v4());
}

::udp::Client::~Client() = default;



// ------------------------------------------------------------------ methods

void ::udp::Client::run()
{
    while (true) {
        ::std::string str;
        ::std::getline(::std::cin, str);
        if (str == "/ping") {
            std::cout << "Server latency: " << this->getServerLatency() << "ms" << ::std::endl;
            continue;
        }
        this->send(::udp::message::Chat{ str });
        this->receive();
        this->handleResponse();
    }
}

void ::udp::Client::send(
    const ::udp::AMessage& message
)
{
    m_socket.send_to(
        ::boost::asio::buffer(&message, sizeof(message)),
        m_receiverEndpoint
    );
}

void ::udp::Client::receive()
{

    ::boost::asio::ip::udp::endpoint senderEndpoint;
    m_socket.receive_from(::boost::asio::buffer(m_buffer), senderEndpoint);
}

void ::udp::Client::handleResponse()
{
    switch (reinterpret_cast<::udp::AMessage*>(&m_buffer)->getHeader().type) {
    case ::udp::AMessage::Header::Type::latency:
        ::std::cout << "[System]: just received a ping" << ::std::endl;
        break;
    case ::udp::AMessage::Header::Type::chatMessage:
        ::std::cout << "Server: " << *reinterpret_cast<udp::message::Chat*>(&m_buffer) << ::std::endl;
        break;
    case ::udp::AMessage::Header::Type::error:
        ::std::cout << reinterpret_cast<udp::message::Error*>(&m_buffer)->toString() << ::std::endl;
        break;
    default:
        ::std::cout << "[System]: unknown response" << ::std::endl;
    }
    if (reinterpret_cast<::udp::AMessage*>(&m_buffer)->isImportant()) {
        this->send(::udp::ping::Confirmation{});
    }
}



// ------------------------------------------------------------------ action

auto ::udp::Client::getServerLatency()
    -> float
{
    this->send(::udp::ping::Latency{});
    auto start = std::chrono::high_resolution_clock::now();
    this->receive();
    auto end = std::chrono::high_resolution_clock::now();
    if (reinterpret_cast<::udp::AMessage*>(&m_buffer)->getHeader().type
        !=::udp::AMessage::Header::Type::latency
    ) {
        throw ::std::runtime_error("expected a ping back from the server");
    }
    return ::std::chrono::duration_cast<::std::chrono::milliseconds>(end - start).count();
}
