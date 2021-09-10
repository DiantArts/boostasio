#pragma once

#include <Udp/Packet/APacket.hpp>



namespace udp::packet {



class Ping
    : public ::udp::APacket
{

public:

    // ------------------------------------------------------------------ *structors

    Ping();

    ~Ping();



private:

};




} // namespace udp::packet
