#pragma once

#include <Packet/APacket.hpp>



namespace packet {



class Ping
    : public ::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    Ping();

    ~Ping();

};



class PingAck
    : public ::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    PingAck();

    ~PingAck();

};



class Connect
    : public ::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    Connect();

    ~Connect();

};



class ConnectionNeeded
    : public ::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    ConnectionNeeded();

    ~ConnectionNeeded();

};



class Disconnect
    : public ::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    Disconnect();

    ~Disconnect();

};




} // namespace packet
