#include <pch.hpp>
#include <Udp/Packet/Loss.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Loss::Loss(
    const ::std::uint8_t lostPacketId
)
    : ::udp::APacket{ ::udp::APacket::Header::Type::ping, 0, false, 0 }
    , m_lostPacketId{ lostPacketId }
{}

::udp::packet::Loss::~Loss() = default;
