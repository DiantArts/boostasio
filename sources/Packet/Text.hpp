#pragma once

#include <Packet/APacket.hpp>



namespace packet {



class Text
    : public ::APacket
{

public:

    using Data = ::std::array<char, APacket::getMaxSize() - APacket::getMaxHeaderSize()>;



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




} // namespace packet
