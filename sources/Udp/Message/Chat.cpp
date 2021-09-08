#include <pch.hpp>
#include <Udp/Message/Chat.hpp>



// ------------------------------------------------------------------ *structors

::udp::message::Chat::Chat(
    const ::std::string& message
)
    : ::udp::AMessage{
        ::udp::AMessage::Header::Type::chatMessage,
        message.size(),
        true
    }
{
    ::std::memmove(m_data.data(), message.data(), message.size());
}

::udp::message::Chat::Chat(
    const ::std::string& message,
    const uint8_t& position
)
    : ::udp::AMessage{
        ::udp::AMessage::Header::Type::chatMessage,
        message.size(),
        position,
        true
    }
{
    ::std::memmove(m_data.data(), message.data(), message.size());
}

::udp::message::Chat::~Chat() = default;



// ------------------------------------------------------------------ data

auto ::udp::message::Chat::getRawData() const
    -> const Chat::Data&
{
    return m_data;
}

auto ::udp::message::Chat::getDataAsString() const
    -> ::std::string
{
    return ::std::string{ m_data.data(), this->getSize() };
}

::udp::message::Chat::operator ::std::string() const
{
    return ::std::string{ m_data.data(), this->getSize() };
}

::udp::message::Chat::operator const char*() const
{
    return static_cast<const char*>(m_data.data());
}
