#include <pch.hpp>
#include <Udp/Packet/APacket.hpp>
#include <Udp/Packet/Text.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::APacket::APacket(
    APacket::Header::Type type,
    ::std::size_t bodySize,
    const bool isImportant,
    const ::std::size_t position
)
    : m_header{
        .type = type,
        .packetId = static_cast<::std::uint8_t>(isImportant ? APacket::generatePacketId() : 0),
        .packetPosition = static_cast<::std::uint8_t>(position),
        .bodySize = static_cast<::std::uint8_t>(bodySize)
    }
{}

::udp::packet::APacket::~APacket() = default;



// ------------------------------------------------------------------ informations

auto ::udp::packet::APacket::getSize() const
    -> ::std::size_t
{
    return this->getMaxHeaderSize() + this->getBodySize();
}

auto ::udp::packet::APacket::getBodySize() const
    -> ::std::size_t
{
    return static_cast<::std::size_t>(m_header.bodySize);
}

auto ::udp::packet::APacket::getHeader() const
    -> const APacket::Header&
{
    return m_header;
}

auto ::udp::packet::APacket::getType() const
    -> APacket::Header::Type
{
    return m_header.type;
}

auto ::udp::packet::APacket::getPosition() const
    -> ::std::uint8_t
{
    return m_header.packetPosition;
}

auto ::udp::packet::APacket::getPacketId() const
    -> ::std::uint8_t
{
    return m_header.packetId;
}

auto ::udp::packet::APacket::isImportant() const
    -> bool
{
    return m_header.packetId != 0;
}

auto ::udp::packet::APacket::getMemoryAddress()
    -> void*
{
    return this;
}



// ------------------------------------------------------------------ debug

void ::udp::packet::APacket::display(
    const char direction[2]
) const
{
    ::std::cout << direction << ' '
        << (int)this->getType() << ' '
        << (int)this->getPosition() << ' '
        << (int)this->getPacketId();
    if (this->getType() == ::udp::packet::APacket::Header::Type::text) {
        ::std::cout << ' ' << reinterpret_cast<const udp::packet::Text*>(this)->toString();
    }
    ::std::cout << '\n';
}



// ------------------------------------------------------------------ newId

void ::udp::packet::APacket::assignNewId()
{
    m_header.packetId = APacket::generatePacketId();
}



// ------------------------------------------------------------------ idGenerator

auto ::udp::packet::APacket::generatePacketId()
    -> ::std::uint8_t
{
    return ++APacket::Header::packetIdGenerator != 0
        ? APacket::Header::packetIdGenerator
        : ++APacket::Header::packetIdGenerator;
}
