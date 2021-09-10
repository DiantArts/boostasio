#include <pch.hpp>
#include <Udp/Packet/Confirmation.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Confirmation::Confirmation()
    : ::udp::APacket{ ::udp::APacket::Header::Type::confirmation, 0, false, 0 }
{}

::udp::packet::Confirmation::~Confirmation() = default;
