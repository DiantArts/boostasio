#pragma once

#include <Udp/Packet/APacket.hpp>



namespace udp::packet {



class Loss
    : public ::udp::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    Loss(
        const ::std::uint8_t lostPacketId
    );

    ~Loss();



private:

    const ::std::uint8_t m_lostPacketId;

};




} // namespace udp::packet
