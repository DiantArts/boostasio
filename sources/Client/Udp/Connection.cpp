#include <pch.hpp>
#include <Client/Udp/Connection.hpp>
#include <Udp/Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::udp::Connection::Connection(
    const ::std::string& host,
    const ::std::string& port,
    const ::std::size_t pingFrequencyAsMs
)
    : m_endpoint{
        ::boost::asio::ip::address::from_string(host),
        static_cast<short unsigned int>(::std::atoi(port.c_str()))
    }
    , m_socket{ m_ioContext }
    , m_pingFrequency{ pingFrequencyAsMs }
{
    m_socket.open(::boost::asio::ip::udp::v4());
}

::udp::Connection::~Connection() = default;



// ------------------------------------------------------------------ methods

auto ::udp::Connection::isValid() const
    -> bool
{
    return m_isRunning;
}

auto ::udp::Connection::getLatency() const
    -> ::std::size_t
{
    return m_latency;
}



// ------------------------------------------------------------------ send

void ::udp::Connection::send(
    const ::udp::APacket& message
)
{
    // debug
    ::std::cout << '\r';
    message.display("->");
    ::std::cout << "Input: " << ::std::flush;

    m_socket.send_to(::boost::asio::buffer(&message, message.getSize()), m_endpoint);
}



// ------------------------------------------------------------------ receive

void ::udp::Connection::startReceive(
    ::std::function<void(::udp::APacket&)>&& func
)
{
    if (m_userReceiveFunc) {
        throw ::std::runtime_error(::std::string(__FUNCTION__) + " has already been call");
    }
    m_userReceiveFunc = ::std::make_unique<::std::function<void(::udp::APacket&)>>(::std::move(func));
    m_receiverThread = ::std::make_unique<::std::thread>(
        [this, &func](){ this->startReceiveImpl(); m_ioContext.run(); }
    );
    m_maintainConnectionThread = ::std::make_unique<::std::thread>(
        ::boost::bind(&Connection::maintainConnection, this)
    );
}

void ::udp::Connection::stopReceive()
{
    m_isRunning = false;
    m_ioContext.stop();
    m_receiverThread->join();
    m_maintainConnectionThread->join();
    m_receiverThread.reset();
    m_maintainConnectionThread.reset();
}

void ::udp::Connection::startReceiveImpl(
)
{
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_endpoint,
        ::boost::bind(&Connection::prehandleMessage, this)
    );
}

void ::udp::Connection::prehandleMessage()
{
    // debug
    ::std::cout << '\r';
    reinterpret_cast<::udp::APacket*>(&m_buffer)->display("<-");
    ::std::cout << "Input: " << ::std::flush;

    // special interactions
    switch (reinterpret_cast<::udp::APacket*>(&m_buffer)->getHeader().type) {
    case ::udp::APacket::Header::Type::ping:
        m_isPingValidated = true;
        m_latency = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_pingTimepoint
        ).count();
        break;
    case ::udp::APacket::Header::Type::error:
        ::std::cout << reinterpret_cast<udp::packet::Error*>(&m_buffer)->toString() << ::std::endl;
        break;
    default:
        // user defined behaviours
        (*m_userReceiveFunc)(*reinterpret_cast<udp::packet::Error*>(&m_buffer));
    }

    this->startReceiveImpl();
}



// ------------------------------------------------------------------ connection

void ::udp::Connection::maintainConnection()
{
    do {
        m_isPingValidated = false;
        this->send(::udp::packet::Ping{});
        m_pingTimepoint = std::chrono::high_resolution_clock::now();
        ::std::this_thread::sleep_for(::std::chrono::milliseconds(m_pingFrequency));
    } while (m_isPingValidated && m_isRunning);
    m_isRunning = false;
    ::std::cout << "\nClient disconnected, press enter to exit..." << ::std::flush;
}
