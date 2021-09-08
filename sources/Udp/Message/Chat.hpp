#pragma once

#include <Udp/AMessage.hpp>



namespace udp::message {



class Chat
    : public ::udp::AMessage
{

public:

    using Data = ::std::array<char, AMessage::Header::maxSize - Chat::getHeaderSize()>;



public:

    // ------------------------------------------------------------------ *structors

    Chat(
        const ::std::string& message
    );

    Chat(
        const ::std::string& message,
        const uint8_t& position
    );

    ~Chat();



    // ------------------------------------------------------------------ data

    [[ nodiscard ]] auto getRawData() const
        -> const Chat::Data&;

    [[ nodiscard ]] auto getDataAsString() const
        -> ::std::string;

    [[ nodiscard ]] operator ::std::string() const;

    [[ nodiscard ]] operator const char*() const;



private:

    Chat::Data m_data;

};




} // namespace udp::message
