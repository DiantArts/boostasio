#include <pch.hpp>
#include <Udp/Packet/Error.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Error::Error(
    Error::Type type
)
    : ::udp::APacket{ ::udp::APacket::Header::Type::error, 0, false, 0 }
    , m_errorType{ type }
{}

::udp::packet::Error::~Error() = default;



// ------------------------------------------------------------------ informations

auto ::udp::packet::Error::getId() const
    -> Error::Type
{
    return m_errorType;
}

auto ::udp::packet::Error::toString() const
    -> ::std::string
{
    return this->errorMessages[static_cast<::std::size_t>(m_errorType)];
}
