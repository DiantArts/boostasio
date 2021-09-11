#include <pch.hpp>
#include <Packet/Ping.hpp>



// ------------------------------------------------------------------ *structors

::packet::Ping::Ping()
    : ::APacket{ ::APacket::Header::Type::ping, 0, true, 0 }
{}

::packet::Ping::~Ping() = default;
