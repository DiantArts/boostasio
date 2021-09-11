#include <pch.hpp>
#include <Packet/Error.hpp>



// ------------------------------------------------------------------ *structors

::packet::Error::Error(
    Error::Type type
)
    : ::APacket{ ::APacket::Header::Type::error, 0, false, 0 }
    , m_errorType{ type }
{}

::packet::Error::~Error() = default;



// ------------------------------------------------------------------ informations

auto ::packet::Error::getId() const
    -> Error::Type
{
    return m_errorType;
}

auto ::packet::Error::toString() const
    -> ::std::string
{
    return this->errorMessages[static_cast<::std::size_t>(m_errorType)];
}
