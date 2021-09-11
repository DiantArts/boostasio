#include <pch.hpp>
#include <Packet/Confirmation.hpp>



// ------------------------------------------------------------------ *structors

::packet::Confirmation::Confirmation()
    : ::APacket{ ::APacket::Header::Type::confirmation, 0, false, 0 }
{}

::packet::Confirmation::~Confirmation() = default;
