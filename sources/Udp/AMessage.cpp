#include <pch.hpp>
#include <Udp/AMessage.hpp>



// ------------------------------------------------------------------ *structors

::udp::AMessage::AMessage(
    AMessage::Header::Type type,
    const ::std::size_t size,
    const bool isImportant
)
    : m_header{
        .type = type,
        .size = static_cast<uint8_t>(size),
        .position = 0,
        .isImportant = isImportant
    }
{}

::udp::AMessage::AMessage(
    AMessage::Header::Type type,
    const ::std::size_t size,
    const uint8_t position,
    const bool isImportant
)
    : m_header{
        .type = type,
        .size = static_cast<uint8_t>(size),
        .position = position,
        .isImportant = isImportant
    }
{}

::udp::AMessage::~AMessage() = default;



// ------------------------------------------------------------------ informations

auto ::udp::AMessage::getHeader() const
    -> const AMessage::Header&
{
    return m_header;
}

auto ::udp::AMessage::getSize() const
    -> ::std::size_t
{
    return m_header.size;
}

auto ::udp::AMessage::getPosition() const
    -> uint8_t
{
    return m_header.position;
}

auto ::udp::AMessage::isImportant() const
    -> bool
{
    return m_header.isImportant;
}

auto ::udp::AMessage::getMemoryAddress()
    -> void*
{
    return this;
}
