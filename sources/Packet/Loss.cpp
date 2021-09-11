#include <pch.hpp>
#include <Packet/Loss.hpp>



// ------------------------------------------------------------------ *structors

::packet::Loss::Loss(
    const ::std::uint8_t lostPacketId
)
    : ::APacket{ ::APacket::Header::Type::ping, 0, false, 0 }
    , m_lostPacketId{ lostPacketId }
{}

::packet::Loss::~Loss() = default;
