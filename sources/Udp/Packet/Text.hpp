#pragma once

#include <Udp/Packet/APacket.hpp>



namespace udp::packet {



class Text
    : public ::udp::APacket
{

public:

    using Data = ::std::array<char, udp::APacket::getMaxSize() - udp::APacket::getMaxHeaderSize()>;



public:

    // ------------------------------------------------------------------ *structors

    Text(
        const ::std::string& message
    );

    Text(
        const ::std::string& message,
        const ::std::size_t& position
    );

    ~Text();



    // ------------------------------------------------------------------ data

    [[ nodiscard ]] auto getRawData() const
        -> const Text::Data&;

    [[ nodiscard ]] auto getDataAsString() const
        -> ::std::string;

    [[ nodiscard ]] operator ::std::string() const;

    [[ nodiscard ]] auto toString() const
        -> ::std::string;



private:

    Text::Data m_data;

};




} // namespace udp::packet
