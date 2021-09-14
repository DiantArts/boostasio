#pragma once

#include <Packet/APacket.hpp>



namespace packet {



class Loss
    : public ::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    Loss(
        ::std::uint8_t lostPacketId
    );

    ~Loss();



    // ------------------------------------------------------------------ comparison

    auto operator<(::std::uint8_t id) const
        -> bool;
    auto operator<=(::std::uint8_t id) const
        -> bool;
    auto operator==(::std::uint8_t id) const
        -> bool;
    auto operator!=(::std::uint8_t id) const
        -> bool;
    auto operator>=(::std::uint8_t id) const
        -> bool;
    auto operator>(::std::uint8_t id) const
        -> bool;



    // ------------------------------------------------------------------ get

    auto getLostPacketId() const
        -> ::std::uint8_t;



private:

    const ::std::uint8_t m_lostPacketId;

};




} // namespace udp::packet
