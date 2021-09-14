#include <pch.hpp>
#include <Packet/PingAck.hpp>



// ------------------------------------------------------------------ *structors

::packet::PingAck::PingAck()
    : ::APacket{ ::packet::Header::Type::pingAck, 0, true, 0 }
{}

::packet::PingAck::~PingAck() = default;
