#include <pch.hpp>
#include <Udp/Message/Error.hpp>



// ------------------------------------------------------------------ *structors

::udp::message::Error::Error(
    Error::Type type
)
    : ::udp::AMessage{ ::udp::AMessage::Header::Type::error, 0, false }
    , m_errorType{ type }
{}

::udp::message::Error::~Error() = default;



// ------------------------------------------------------------------ informations

auto ::udp::message::Error::getId() const
    -> Error::Type
{
    return m_errorType;
}

auto ::udp::message::Error::toString() const
    -> ::std::string
{
    return this->errorMessages[static_cast<::std::size_t>(m_errorType)];
}
