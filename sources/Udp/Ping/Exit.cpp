#include <pch.hpp>
#include <Udp/Ping/Exit.hpp>



// ------------------------------------------------------------------ *structors

::udp::ping::Exit::Exit()
    : ::udp::AMessage{ ::udp::AMessage::Header::Type::exit, 0, true }
{}

::udp::ping::Exit::~Exit() = default;
