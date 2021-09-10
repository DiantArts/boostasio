#include <pch.hpp>
#include <Udp/Packet/Exit.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Exit::Exit()
    : ::udp::APacket{ ::udp::APacket::Header::Type::exit, 0, true, 0 }
{}

::udp::packet::Exit::~Exit() = default;
