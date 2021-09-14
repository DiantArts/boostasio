#include <pch.hpp>
#include <Packet/Text.hpp>



// ------------------------------------------------------------------ *structors

::packet::Text::Text(
    const ::std::string& message
)
    : ::APacket{ ::packet::Header::Type::text, message.size(), true, 0 }
{
    ::std::memmove(m_data.data(), message.data(), message.size());
}

::packet::Text::Text(
    const ::std::string& message,
    const ::std::size_t& position
)
    : ::APacket{ ::packet::Header::Type::text, message.size(), true, position }
{
    ::std::memmove(m_data.data(), message.data(), message.size());
}

::packet::Text::~Text() = default;



// ------------------------------------------------------------------ data

auto ::packet::Text::getRawData() const
    -> const Text::Data&
{
    return m_data;
}

auto ::packet::Text::getDataAsString() const
    -> ::std::string
{
    return ::std::string{ m_data.data(), this->getBodySize() };
}

::packet::Text::operator ::std::string() const
{
    return ::std::string{ m_data.data(), this->getBodySize() };
}

[[ nodiscard ]] auto ::packet::Text::toString() const
    -> ::std::string
{
    return ::std::string{ m_data.data(), this->getBodySize() };
}
