#include <pch.hpp>
#include <Packet/Exit.hpp>



// ------------------------------------------------------------------ *structors

::packet::Exit::Exit()
    : ::APacket{ ::APacket::Header::Type::exit, 0, true, 0 }
{}

::packet::Exit::~Exit() = default;
