#include <pch.hpp>
#include <Udp/Ping/Confirmation.hpp>



// ------------------------------------------------------------------ *structors

::udp::ping::Confirmation::Confirmation()
    : ::udp::AMessage{ ::udp::AMessage::Header::Type::confirmation, 0, false }
{}

::udp::ping::Confirmation::~Confirmation() = default;
