#include <pch.hpp>
#include <Udp/Packet/Ping.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Ping::Ping()
    : ::udp::APacket{ ::udp::APacket::Header::Type::ping, 0, true, 0 }
{}

::udp::packet::Ping::~Ping() = default;
