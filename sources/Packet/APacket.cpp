#include <pch.hpp>
#include <Packet/APacket.hpp>
#include <Packet/Text.hpp>
#include <Packet/Loss.hpp>



// ------------------------------------------------------------------ *structors

::packet::APacket::APacket(
    packet::Header::Type type,
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

::packet::APacket::~APacket() = default;



// ------------------------------------------------------------------ informations

auto ::packet::APacket::getSize() const
    -> ::std::size_t
{
    return this->getMaxHeaderSize() + this->getBodySize();
}

auto ::packet::APacket::getBodySize() const
    -> ::std::size_t
{
    return static_cast<::std::size_t>(m_header.bodySize);
}

auto ::packet::APacket::getHeader() const
    -> const packet::Header&
{
    return m_header;
}

auto ::packet::APacket::getType() const
    -> packet::Header::Type
{
    return m_header.type;
}

auto ::packet::APacket::getPosition() const
    -> ::std::uint8_t
{
    return m_header.packetPosition;
}

auto ::packet::APacket::getId() const
    -> ::std::uint8_t
{
    return m_header.packetId;
}

auto ::packet::APacket::isImportant() const
    -> bool
{
    return m_header.packetId != 0;
}

auto ::packet::APacket::getMemoryAddress()
    -> void*
{
    return this;
}



// ------------------------------------------------------------------ debug

void ::packet::APacket::display(
    const char direction[2]
) const
{
    ::std::cout << direction << ' '
        << (int)this->getType() << ' '
        << (int)this->getPosition() << ' '
        << (int)this->getId();
    if (this->getType() == ::packet::Header::Type::text) {
        ::std::cout << ' ' << reinterpret_cast<const ::packet::Text*>(this)->toString();
    } else if (this->getType() == ::packet::Header::Type::packetLoss) {
        ::std::cout << ' ' << (int)reinterpret_cast<const ::packet::Loss*>(this)->getLostPacketId();
    }
    ::std::cout << '\n';
}



// ------------------------------------------------------------------ newId

void ::packet::APacket::assignNewId()
{
    m_header.packetId = APacket::generatePacketId();
}



// ------------------------------------------------------------------ idGenerator

auto ::packet::APacket::generatePacketId()
    -> ::std::uint8_t
{
    return ++packet::Header::packetIdGenerator != 0
        ? packet::Header::packetIdGenerator
        : ++packet::Header::packetIdGenerator;
}
