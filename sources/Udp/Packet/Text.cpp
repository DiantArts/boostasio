#include <pch.hpp>
#include <Udp/Packet/Text.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Text::Text(
    const ::std::string& message
)
    : ::udp::APacket{ ::udp::APacket::Header::Type::text, message.size(), true, 0 }
{
    ::std::memmove(m_data.data(), message.data(), message.size());
}

::udp::packet::Text::Text(
    const ::std::string& message,
    const ::std::size_t& position
)
    : ::udp::APacket{ ::udp::APacket::Header::Type::text, message.size(), true, position }
{
    ::std::memmove(m_data.data(), message.data(), message.size());
}

::udp::packet::Text::~Text() = default;



// ------------------------------------------------------------------ data

auto ::udp::packet::Text::getRawData() const
    -> const Text::Data&
{
    return m_data;
}

auto ::udp::packet::Text::getDataAsString() const
    -> ::std::string
{
    return ::std::string{ m_data.data(), this->getBodySize() };
}

::udp::packet::Text::operator ::std::string() const
{
    return ::std::string{ m_data.data(), this->getBodySize() };
}

[[ nodiscard ]] auto ::udp::packet::Text::toString() const
    -> ::std::string
{
    return ::std::string{ m_data.data(), this->getBodySize() };
}
