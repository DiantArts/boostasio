#include <pch.hpp>
#include <Buffer.hpp>



// ------------------------------------------------------------------ *structors

::Buffer::Buffer(
    const int port
)
    : m_socket{ m_ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
{}

::Buffer::Buffer(
    const ::std::string& host,
    const int port
)
    : m_endpoint{
        ::boost::asio::ip::address::from_string(host),
        static_cast<short unsigned int>(port)
    }
    , m_socket{ m_ioContext }
{
    m_socket.open(::boost::asio::ip::udp::v4());
}

::Buffer::~Buffer() = default;



// ------------------------------------------------------------------ send

void ::Buffer::send(
    ::std::unique_ptr<::APacket>&& message,
    ::boost::asio::ip::udp::endpoint endpoint
)
{
    // debug
    message->display("->");

    m_socket.send_to(::boost::asio::buffer(message.get(), message->getSize()), endpoint);
    m_sentPackets.push(::std::move(message));
}

void ::Buffer::reply(
    ::std::unique_ptr<::APacket>&& message
)
{
    // debug
    message->display("->");

    m_socket.send_to(::boost::asio::buffer(message.get(), message->getSize()), m_endpoint);
    m_sentPackets.push(::std::move(message));
}



// ------------------------------------------------------------------ packets

void ::Buffer::clearSentPackets()
{
    std::queue<::std::unique_ptr<::APacket>>{}.swap(m_sentPackets);
}

auto ::Buffer::isPacketSent(
    ::std::uint8_t packetId
) -> bool
{
    auto& packetLost{ *reinterpret_cast<::packet::Loss*>(&m_buffer) };
    while (!m_sentPackets.empty()) {
        if (packetLost.getLostPacketId() == packetId) {
            m_sentPackets.pop();
            return true;
        }
        m_sentPackets.pop();
    }
    return false;
}
