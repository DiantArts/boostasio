#include <pch.hpp>
#include <Udp/Packet/Syn.hpp>



// ------------------------------------------------------------------ *structors

::udp::packet::Syn::Syn()
    : ::udp::APacket{ ::udp::APacket::Header::Type::syn, 0, true, 0 }
{}

::udp::packet::Syn::~Syn() = default;
