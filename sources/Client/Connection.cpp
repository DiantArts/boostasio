#include <pch.hpp>
#include <Client/Connection.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::client::Connection::Connection(
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

::client::Connection::~Connection() = default;



// ------------------------------------------------------------------ methods

auto ::client::Connection::isValid() const
    -> bool
{
    return m_isRunning;
}

auto ::client::Connection::getLatency() const
    -> ::std::size_t
{
    return m_latency;
}



// ------------------------------------------------------------------ send

void ::client::Connection::send(
    ::std::unique_ptr<::APacket>&& message
)
{
    // debug
    ::std::cout << '\r';
    message->display("->");
    ::std::cout << "Input: " << ::std::flush;

    m_socket.send_to(::boost::asio::buffer(message.get(), message->getSize()), m_endpoint);
    m_sentPackets.push(::std::move(message));
}



// ------------------------------------------------------------------ receive

void ::client::Connection::startReceive(
    ::std::function<void(::APacket&)>&& func
)
{
    if (m_userReceiveFunc) {
        throw ::std::runtime_error(::std::string(__FUNCTION__) + " has already been call");
    }
    m_userReceiveFunc = ::std::make_unique<::std::function<void(::APacket&)>>(::std::move(func));
    m_receiverThread = ::std::make_unique<::std::thread>(
        [this, &func](){ this->startReceiveImpl(); m_ioContext.run(); }
    );
    m_maintainConnectionThread = ::std::make_unique<::std::thread>(
        ::boost::bind(&Connection::maintainConnection, this)
    );
}

void ::client::Connection::stopReceive()
{
    m_isRunning = false;
    m_ioContext.stop();
    m_receiverThread->join();
    m_maintainConnectionThread->join();
    m_receiverThread.reset();
    m_maintainConnectionThread.reset();
}

void ::client::Connection::startReceiveImpl(
)
{
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_endpoint,
        ::boost::bind(&Connection::prehandleMessage, this)
    );
}

void ::client::Connection::prehandleMessage()
{
    auto& message = *reinterpret_cast<::APacket*>(&m_buffer);

    // debug
    ::std::cout << '\r';
    message.display("<-");
    ::std::cout << "Input: " << ::std::flush;

    // special interactions
    switch (message.getHeader().type) {
    case ::packet::Header::Type::packetLoss: {

        // search for the lost packet in the most recent sent packet
        auto& packetLost{ *reinterpret_cast<::packet::Loss*>(&m_buffer) };
        while (!m_sentPackets.empty()) {
            if (packetLost.getLostPacketId() == m_sentPackets.front()->getId()) {
                m_sentPackets.front()->assignNewId();
                this->send(::std::move(m_sentPackets.front()));
                m_sentPackets.pop();
                break;
            }
            m_sentPackets.pop();
        }
        break;

    } case ::packet::Header::Type::ping:

        // compute the latency and confirm that the client is still connected to the server
        m_isPingValidated = true;
        m_latency = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - m_pingTimepoint
        ).count();

        // clear the queue because we have the confirmation that no packet has been lost
        std::queue<::std::unique_ptr<::APacket>>{}.swap(m_sentPackets);

        break;
    case ::packet::Header::Type::error:
        ::std::cout << reinterpret_cast<::packet::Error*>(&m_buffer)->toString() << ::std::endl;
        break;
    default:
        // user defined behaviours
        (*m_userReceiveFunc)(message);
    }

    this->startReceiveImpl();
}



// ------------------------------------------------------------------ connection

void ::client::Connection::maintainConnection()
{
    do {
        m_isPingValidated = false;
        this->send(::std::make_unique<::packet::Ping>());
        m_pingTimepoint = std::chrono::high_resolution_clock::now();
        ::std::this_thread::sleep_for(::std::chrono::milliseconds(m_pingFrequency));
    } while (m_isPingValidated && m_isRunning);
    m_isRunning = false;
    ::std::cout << "\nClient disconnected, press enter to exit..." << ::std::flush;
}
