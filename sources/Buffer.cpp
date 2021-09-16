#include <pch.hpp>
#include <Buffer.hpp>



// ------------------------------------------------------------------ *structors

::Buffer::Buffer(
    ::boost::asio::io_context ioContext,
    const ::std::string& host,
    const int port
)
    : m_endpoint{ ::boost::asio::ip::address::from_string(host), static_cast<short unsigned>(port) }
    , m_socket{ ioContext }
{
    m_socket.open(::boost::asio::ip::udp::v4());
}

::Buffer::Buffer(
    ::boost::asio::io_context ioContext,
    int port
)
    : m_socket{ ioContext, ::boost::asio::ip::udp::endpoint(::boost::asio::ip::udp::v4(), port) }
{}

::Buffer::~Buffer() = default;



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
            return true;
        }
        m_sentPackets.pop();
    }
    return false;
}



// ------------------------------------------------------------------ getters

auto ::Buffer::get()
    -> ::std::array<::std::byte, Buffer::bufferLength>&
{
    return m_buffer;
}

::Buffer::operator ::APacket&()
{
    return *reinterpret_cast<::APacket*>(&m_buffer);
}



// ------------------------------------------------------------------ send

void ::Buffer::send(
    ::std::unique_ptr<::APacket>&& message,
    ::boost::asio::ip::udp::endpoint endpoint
)
{
    // debug
    message->display("->");

    m_socket.send_to(
        ::boost::asio::buffer(message.get(), message->getSize()), m_endpoint = ::std::move(endpoint)
    );
    m_sentPackets.push(::std::move(message));
}
