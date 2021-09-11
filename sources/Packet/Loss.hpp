#pragma once

#include <Packet/APacket.hpp>



namespace packet {



class Loss
    : public ::APacket
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
