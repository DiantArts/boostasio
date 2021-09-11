#include <pch.hpp>
#include <Packet/Syn.hpp>



// ------------------------------------------------------------------ *structors

::packet::Syn::Syn()
    : ::APacket{ ::APacket::Header::Type::syn, 0, true, 0 }
{}

::packet::Syn::~Syn() = default;
