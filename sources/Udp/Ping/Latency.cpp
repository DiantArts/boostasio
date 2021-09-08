#include <pch.hpp>
#include <Udp/Ping/Latency.hpp>



// ------------------------------------------------------------------ *structors

::udp::ping::Latency::Latency()
    : ::udp::AMessage{ ::udp::AMessage::Header::Type::latency, 0, true }
{}

::udp::ping::Latency::~Latency() = default;
