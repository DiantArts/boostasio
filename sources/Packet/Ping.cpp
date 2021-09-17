#include <pch.hpp>
#include <Packet/Ping.hpp>



// ------------------------------------------------------------------ Ping

::packet::Ping::Ping()
    : ::APacket{ ::packet::Header::Type::ping, 0, true, 0 }
{}

::packet::Ping::~Ping() = default;

// ------------------------------------------------------------------ PingAck

::packet::PingAck::PingAck()
    : ::APacket{ ::packet::Header::Type::pingAck, 0, true, 0 }
{}

::packet::PingAck::~PingAck() = default;



// ------------------------------------------------------------------ Connect

::packet::Connect::Connect()
    : ::APacket{ ::packet::Header::Type::connect, 0, true, 0 }
{}

::packet::Connect::~Connect() = default;



// ------------------------------------------------------------------ ConnectionNeeded

::packet::ConnectionNeeded::ConnectionNeeded()
    : ::APacket{ ::packet::Header::Type::connectionNeeded, 0, true, 0 }
{}

::packet::ConnectionNeeded::~ConnectionNeeded() = default;



// ------------------------------------------------------------------ Disconnect

::packet::Disconnect::Disconnect()
    : ::APacket{ ::packet::Header::Type::disconnect, 0, true, 0 }
{}

::packet::Disconnect::~Disconnect() = default;
